#include "tcpconn.h"
#include "log.h"
#include "sockets.h"

namespace RACLE {

TcpConnection::TcpConnection(UserEnvironment *env, int clientFd)
    : mEnv(env), mClientFd(clientFd) {
    mClientIOEvent = IOEvent::createNew(clientFd, this);
    mClientIOEvent->setReadCallback(readCallback);
    mClientIOEvent->setWriteCallback(writeCallback);
    mClientIOEvent->setErrorCallback(errorCallback);
    mClientIOEvent->enableReadHandling(); // 默认只开启读

    //    mClientIOEvent->enableWriteHandling();
    //    mClientIOEvent->enableErrorHandling();

    mEnv->scheduler()->addIOEvent(mClientIOEvent);
}

TcpConnection::~TcpConnection() {
    mEnv->scheduler()->removeIOEvent(mClientIOEvent);
    delete mClientIOEvent;
    //    Delete::release(mClientIOEvent);

    sockets::close(mClientFd);
}

void TcpConnection::setDisConnectCallback(DisConnectCallback cb, void *arg) {
    mDisConnectCallback = cb;
    mArg = arg;
}

void TcpConnection::enableReadHandling() {
    if (mClientIOEvent->isReadHandling()) return;
    mClientIOEvent->enableReadHandling();
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::enableWriteHandling() {
    if (mClientIOEvent->isWriteHandling()) return;
    mClientIOEvent->enableWriteHandling();
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::enableErrorHandling() {
    if (mClientIOEvent->isErrorHandling()) return;
    mClientIOEvent->enableErrorHandling();
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::disableReadeHandling() {
    if (!mClientIOEvent->isReadHandling()) return;
    mClientIOEvent->disableReadeHandling();
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::disableWriteHandling() {
    if (!mClientIOEvent->isWriteHandling()) return;
    mClientIOEvent->disableWriteHandling();
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::disableErrorHandling() {
    if (!mClientIOEvent->isErrorHandling()) return;
    mClientIOEvent->disableErrorHandling();
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::handleRead() {
    int ret = mInputBuffer.read(mClientFd);

    if (ret <= 0) {
        LOGE("read error,fd=%d,ret=%d", mClientFd, ret);
        handleDisConnect();
        return;
    }

    handleReadBytes();
}

void TcpConnection::handleReadBytes() {
    // LOGI("");
    mInputBuffer.retrieveAll();
}

void TcpConnection::handleDisConnect() {
    if (mDisConnectCallback) { mDisConnectCallback(mArg, mClientFd); }
}

void TcpConnection::handleWrite() {
    // LOGI("");
    mOutBuffer.retrieveAll();
}

void TcpConnection::handleError() {
    // LOGI("");
}

void TcpConnection::readCallback(void *arg) {
    auto *tcpConnection = (TcpConnection *)arg;
    tcpConnection->handleRead();
}

void TcpConnection::writeCallback(void *arg) {
    auto *tcpConnection = (TcpConnection *)arg;
    tcpConnection->handleWrite();
}

void TcpConnection::errorCallback(void *arg) {
    auto *tcpConnection = (TcpConnection *)arg;
    tcpConnection->handleError();
}

} // namespace RACLE