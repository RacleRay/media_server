#ifndef __TCPCONN__H__
#define __TCPCONN__H__

#include "buffer.h"
#include "event.h"
#include "uenv.h"

namespace RACLE {

class TcpConnection {
  public:
    using DisConnectCallback = void (*)(void *, int);
    // typedef void (*DisConnectCallback)(void *, int);

    TcpConnection(UserEnvironment *env, int clientFd);
    virtual ~TcpConnection();

    void setDisConnectCallback(DisConnectCallback cb, void *arg);

  protected:
    void enableReadHandling();
    void enableWriteHandling();
    void enableErrorHandling();

    void disableReadeHandling();
    void disableWriteHandling();
    void disableErrorHandling();

    void handleRead();

    virtual void handleReadBytes();

    virtual void handleWrite();

    virtual void handleError();

    void handleDisConnect();

  private:
    static void readCallback(void *arg);
    static void writeCallback(void *arg);
    static void errorCallback(void *arg);

  protected:
    UserEnvironment *mEnv;

    int mClientFd;
    IOEvent *mClientIOEvent;
    DisConnectCallback mDisConnectCallback;

    void *mArg;

    Buffer mInputBuffer;
    Buffer mOutBuffer;

    // 组织临时消息时使用
    char mBuffer[2048];
};

} // namespace RACLE

#endif //!__TCPCONN__H__