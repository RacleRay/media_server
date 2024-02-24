#ifndef __MEDIASSESION__H__
#define __MEDIASSESION__H__

#include <list>
#include <string>

#include "rtpinstance.h"
#include "sink.h"

#define MEDIA_MAX_TRACK_NUM 2


class MediaSession {

public:
    enum TrackId {
        TrackNone = -1,
        TrackId0 = 0,
        TrackId1 = 1,
    };

public:
    explicit MediaSession(const std::string& sessionName);
    ~MediaSession();

    // singleton (simple way)
    static MediaSession *createNewSession(std::string sessionName);

    [[nodiscard]]   
    std::string name() const { return mSessionName; }

    std::string generateSDPDescription();

    // producer
    bool addSink(MediaSession::TrackId trackId, Sink* sink);
    bool addRtpInstance(MediaSession::TrackId trackId, RtpInstance* rtpInstance);

    // remove consumer 
    bool removeRtpInstance(RtpInstance* rtpInstance);

    // 多播
    bool startMulticast();
    bool isStartMulticast();

    [[nodiscard]]
    std::string getMulticastDestAddr() const { return mMulticastAddr; }
    uint16_t getMulticastDestRtpPort(TrackId trackId);

private:
    struct Track {
        bool mIsAlive;
        Sink* mSink;
        int mTrackId;
        std::list<RtpInstance*> mRtpInstances;
    };    

    Track* getTrack(MediaSession::TrackId trackId);

    // private callbacks
    static void sendPacketCallback();

    void handleSendRtpPacket();

private:
    std::string mSessionName;
    
    std::string mSDP;

    // multicast related
    bool mIsMulticast;
    std::string mMulticastAddr;
    RtpInstance* mMulticastRtpInstances[MEDIA_MAX_TRACK_NUM];
    RtcpInstance* mMulticastRtcpInstances[MEDIA_MAX_TRACK_NUM];

};

#endif //!__MEDIASSESION__H__