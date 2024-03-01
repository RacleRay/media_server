#ifndef __RTSPSERVER__H__
#define __RTSPSERVER__H__

#include <mutex>

#include "address.h"
#include "event.h"
#include "mediasession.h"
#include "uenv.h"

namespace RACLE {

class MediaSessionManager;
class RtspConnection;
class RtspServer {
  public:
    static RtspServer *createNew(
        UserEnvironment *env, MediaSessionManager *sessMgr, IPv4Address &addr);

    RtspServer(
        UserEnvironment *env, MediaSessionManager *sessMgr, IPv4Address &addr);
    ~RtspServer();

  public:
    MediaSessionManager *mSessMgr;
    void start();

    [[nodiscard]] UserEnvironment *env() const { return mEnv; }

  private:
    static void readCallback(void *);
    void handleRead();
    static void cbDisConnect(void *arg, int clientFd);
    void handleDisConnect(int clientFd);
    static void cbCloseConnect(void *arg);
    void handleCloseConnect();

  private:
    bool mListen;
    UserEnvironment *mEnv;
    int mFd;
    IPv4Address mAddr;
    IOEvent *mAcceptIOEvent;
    std::mutex mMtx;

    std::map<int, RtspConnection *> mConnMap;
    std::vector<int> mDisConnList;

    TriggerEvent *mCloseTriggerEvent;
};

} // namespace RACLE

#endif //!__RTSPSERVER__H__