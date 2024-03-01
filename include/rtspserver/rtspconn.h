#ifndef __RTSPCONN__H__
#define __RTSPCONN__H__

#include <map>
#include <string>

#include "mediasession.h"
#include "tcpconn.h"

namespace RACLE {
class RtspServer;

class RtspConnection : public TcpConnection {
  public:
    enum Method {
        OPTIONS,
        DESCRIBE,
        SETUP,
        PLAY,
        TEARDOWN,
        NONE,
    };

    static RtspConnection *createNew(RtspServer *rtspServer, int clientFd);

    RtspConnection(RtspServer *rtspServer, int clientFd);
    ~RtspConnection() override;

  protected:
    void handleReadBytes() override;

  private:
    bool parseRequest();
    bool parseRequest1(const char *begin, const char *end);
    bool parseRequest2(const char *begin, const char *end);

    bool parseCSeq(std::string &message);
    bool parseDescribe(std::string &message);
    bool parseSetup(std::string &message);
    bool parsePlay(std::string &message);

    bool handleCmdOption();
    bool handleCmdDescribe();
    bool handleCmdSetup();
    bool handleCmdPlay();
    bool handleCmdTeardown();

    int sendMessage(void *buf, int size);
    int sendMessage();

    bool createRtpRtcpOverUdp(
        MediaSession::TrackId trackId,
        std::string peerIp,
        uint16_t peerRtpPort,
        uint16_t peerRtcpPort);
    bool createRtpOverTcp(
        MediaSession::TrackId trackId, int sockfd, uint8_t rtpChannel);

    void handleRtpOverTcp();

  private:
    RtspServer *mRtspServer;
    std::string mPeerIp;
    Method mMethod;
    std::string mUrl;
    std::string mSuffix;
    uint32_t mCSeq;
    std::string mStreamPrefix;

    uint16_t mPeerRtpPort;
    uint16_t mPeerRtcpPort;

    MediaSession::TrackId mTrackId;
    RtpInstance *mRtpInstances[MEDIA_MAX_TRACK_NUM];
    RtcpInstance *mRtcpInstances[MEDIA_MAX_TRACK_NUM];

    int mSessionId;
    bool mIsRtpOverTcp;
    uint8_t mRtpChannel;
};

} // namespace RACLE

#endif //!__RTSPCONN__H__