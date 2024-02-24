#ifndef __RTPINSTANCE__H__
#define __RTPINSTANCE__H__

#include <cstdint>
#include <string>

#include "address.h"
#include "rtp.h"
#include "sockets.h"

class RtpInstance {
  public:
    enum RtpType {
        RTP_OVER_UDP,
        RTP_OVER_TCP,
    };

    RtpInstance(
        int localSockfd,
        uint16_t localPort,
        const std::string &destIp,
        uint16_t destPort)
        : mRtpType(RTP_OVER_UDP)
        , mSockfd(localSockfd)
        , mLocalPort(localPort)
        , mDestAddr(destIp, destPort)
        , mIsAlive(false)
        , mSessionId(0)
        , mRtpChannel(0) {}

    RtpInstance(int sockfd, uint8_t rtpChannel)
        : mRtpType(RTP_OVER_TCP)
        , mSockfd(sockfd)
        , mLocalPort(0)
        , mIsAlive(false)
        , mSessionId(0)
        , mRtpChannel(rtpChannel) {}

    static RtpInstance *createNewOverUdp(
        int localSockfd,
        uint16_t localPort,
        const std::string &destIp,
        uint16_t destPort) 
    {
        return new RtpInstance(localSockfd, localPort, destIp, destPort);
    }

    static RtpInstance *createNewOverTcp(int sockfd, uint8_t rtpChannel) {
        return new RtpInstance(sockfd, rtpChannel);
    }

  private:
    uint16_t mLocalPort;   // for udp
    IPv4Address mDestAddr; // for udp

    RtpType mRtpType;
    int mSockfd;
    bool mIsAlive;
    uint16_t mSessionId;
    uint8_t mRtpChannel;
};

class RtcpInstance {
  public:
    static RtcpInstance *createNew(
        int localSockfd,
        uint16_t localPort,
        const std::string &destIp,
        uint16_t destPort) {
        return new RtcpInstance(localSockfd, localPort, destIp, destPort);
    }

    ssize_t send(void *buf, int size) {
        return sockets::sendto(mLocalSockfd, buf, size, mDestAddr.getAddr());
    }

    int recv(void *buf, int size, IPv4Address *addr) {
        // nothing to do currently.
        return 0;
    }

    [[nodiscard]]
    uint16_t getLocalPort() const { return mLocalPort; }

    [[nodiscard]]
    int alive() const { return mIsAlive; }
    
    int setAlive(bool alive) {
        mIsAlive = alive;
        return 0;
    };
    
    void setSessionId(uint16_t sessionId) { mSessionId = sessionId; }
    
    [[nodiscard]]
    uint16_t sessionId() const { return mSessionId; }

  public:
    RtcpInstance(
        int localSockfd,
        uint16_t localPort,
        const std::string &destIp,
        uint16_t destPort)
        : mLocalSockfd(localSockfd)
        , mLocalPort(localPort)
        , mDestAddr(destIp, destPort) {}

    ~RtcpInstance() { sockets::close(mLocalSockfd); }

  private:
    int mLocalSockfd;
    uint16_t mLocalPort;
    IPv4Address mDestAddr;
    bool mIsAlive{false};
    uint16_t mSessionId{0};
};

#endif //!__RTPINSTANCE__H__