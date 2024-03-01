#include "sink.h"
#include "log.h"
#include "sockets.h"

namespace RACLE {

Sink::Sink(UserEnvironment *env, MediaSource *mediaSource, int payloadType)
    : mMediaSource(mediaSource)
    , mEnv(env)
    , mCsrcLen(0)
    , mExtension(0)
    , mPadding(0)
    , mVersion(RTP_VERSION)
    , mPayloadType(payloadType)
    , mMarker(0)
    , mSeq(0)
    , mSSRC(rand())
    , mTimestamp(0)
    , mTimerId(0)
    , mSessionSendPacket(nullptr)
    , mArg1(nullptr)
    , mArg2(nullptr) {
    LOGI("Sink()");
    mTimerEvent = TimerEvent::createNew(this);
    mTimerEvent->setTimeoutCallback(cbTimeout);
}

Sink::~Sink() {
    LOGI("~Sink()");

    delete mTimerEvent;
    delete mMediaSource;
}

void Sink::stopTimerEvent() {
    mTimerEvent->stop();
}

// arg1: MediaSession*, arg2: Track*
void Sink::setSessionCb(SessionSendPacketCallback cb, void *arg1, void *arg2) {
    mSessionSendPacket = cb;
    mArg1 = arg1;
    mArg2 = arg2;
}

void Sink::sendRtpPacket(RtpPacket *packet) {
    RtpHeader *rtpHeader = packet->mRtpHeader;
    rtpHeader->csrcLen = mCsrcLen;
    rtpHeader->extension = mExtension;
    rtpHeader->padding = mPadding;
    rtpHeader->version = mVersion;
    rtpHeader->payloadType = mPayloadType;
    rtpHeader->marker = mMarker;
    rtpHeader->seq = htons(mSeq);
    rtpHeader->timestamp = htonl(mTimestamp);
    rtpHeader->ssrc = htonl(mSSRC);

    if (mSessionSendPacket) {
        mSessionSendPacket(mArg1, mArg2, packet, PacketType::RTPPACKET);
    }
}

void Sink::cbTimeout(void *arg) {
    Sink *sink = (Sink *)arg;
    sink->handleTimeout();
}

void Sink::handleTimeout() {
    MediaFrame *frame = mMediaSource->getFrameFromOutputQueue();
    if (!frame) { return; }
    this->sendFrame(frame);
    mMediaSource->putFrameToInputQueue(frame);
}

void Sink::runEvery(int interval) {
    mTimerId = mEnv->scheduler()->addTimedEventRunEvery(mTimerEvent, interval);
}

} // namespace RACLE
