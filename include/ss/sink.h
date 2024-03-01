#ifndef __SINK__H__
#define __SINK__H__

#include <cstdint>
#include <string>

#include "event.h"
#include "rtp.h"
#include "source.h"
#include "uenv.h"

namespace RACLE {

class Sink {
  public:
    enum PacketType {
        UNKNOWN = -1,
        RTPPACKET = 0,
    };

    typedef void (*SessionSendPacketCallback)(
        void *arg1, void *arg2, void *packet, PacketType packetType);

    Sink(UserEnvironment *env, MediaSource *mediaSource, int payloadType);
    virtual ~Sink();

    void stopTimerEvent();

    virtual std::string getMediaDescription(uint16_t port) = 0;
    virtual std::string getAttribute() = 0;

    void setSessionCb(SessionSendPacketCallback cb, void *arg1, void *arg2);

  protected:
    virtual void sendFrame(MediaFrame *frame) = 0;
    void sendRtpPacket(RtpPacket *packet);

    void runEvery(int interval);

  private:
    static void cbTimeout(void *arg);
    void handleTimeout();

  protected:
    UserEnvironment *mEnv;
    MediaSource *mMediaSource;
    SessionSendPacketCallback mSessionSendPacket;
    void *mArg1;
    void *mArg2;

    uint8_t mCsrcLen;
    uint8_t mExtension;
    uint8_t mPadding;
    uint8_t mVersion;
    uint8_t mPayloadType;
    uint8_t mMarker;
    uint16_t mSeq;
    uint32_t mTimestamp;
    uint32_t mSSRC;

  private:
    TimerEvent *mTimerEvent;
    Timer::TimerId mTimerId;
};

} // namespace RACLE

#endif //!__SINK__H__