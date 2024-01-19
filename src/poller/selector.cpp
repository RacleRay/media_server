#include "selector.h"

namespace RACLE {

SelectPoller::SelectPoller() {
    FD_ZERO(&mReadSet);
    FD_ZERO(&mWriteSet);
    FD_ZERO(&mExceptionSet);
}

SelectPoller *SelectPoller::createNew() {
    return new SelectPoller();
}

bool SelectPoller::addIOEvent(IOEvent *event) {
    return updateIOEvent(event);
}

bool SelectPoller::updateIOEvent(IOEvent *event) {
    int fd = event->getFd();
    if (fd < 0) {
        LOGE("fd=%d", fd);

        return false;
    }

    FD_CLR(fd, &mReadSet);
    FD_CLR(fd, &mWriteSet);
    FD_CLR(fd, &mExceptionSet);

    auto it = mIOEventMap.find(fd);
    if (it != mIOEventMap.end()) // 先前已经添加则修改
    {
        if (event->isReadHandling()) FD_SET(fd, &mReadSet);
        if (event->isWriteHandling()) FD_SET(fd, &mWriteSet);
        if (event->isErrorHandling()) FD_SET(fd, &mExceptionSet);
    } else // 先前未添加则添加IO事件
    {
        if (event->isReadHandling()) FD_SET(fd, &mReadSet);
        if (event->isWriteHandling()) FD_SET(fd, &mWriteSet);
        if (event->isErrorHandling()) FD_SET(fd, &mExceptionSet);

        mIOEventMap.insert(std::make_pair(fd, event));
    }

    if (mIOEventMap.empty()) {
        mMaxNumSockets = 0;
    } else {
         // 更新最大文件描述符+1（map会自动排序）
        mMaxNumSockets = mIOEventMap.rbegin()->first + 1;
    }

    return true;
}

bool SelectPoller::removeIOEvent(IOEvent *event) {
    int fd = event->getFd();
    if (fd < 0) { 
        return false;
    }

    FD_CLR(fd, &mReadSet);
    FD_CLR(fd, &mWriteSet);
    FD_CLR(fd, &mExceptionSet);

    auto it = mIOEventMap.find(fd);
    if (it != mIOEventMap.end()) {
        mIOEventMap.erase(it);
    }

    if (mIOEventMap.empty()) {
        mMaxNumSockets = 0;
    }
    else {
        mMaxNumSockets = mIOEventMap.rbegin()->first + 1;        
    }

    return true;
}

void SelectPoller::handleEvent() {
    fd_set readSet = mReadSet;
    fd_set writeSet = mWriteSet;
    fd_set exceptionSet = mExceptionSet;
    struct timeval timeout;
    int ret;
    int rEvent;

    timeout.tv_sec = 1000; // 秒
    timeout.tv_usec = 0;   // 微秒

    ret = select(mMaxNumSockets, &readSet, &writeSet, &exceptionSet, &timeout);

    if (ret < 0) {
        return;
    }

    for (auto it = mIOEventMap.begin(); it != mIOEventMap.end(); ++it) {
        rEvent = 0;
        if (FD_ISSET(it->first, &readSet)) rEvent |= IOEvent::EVENT_READ;
        if (FD_ISSET(it->first, &writeSet)) rEvent |= IOEvent::EVENT_WRITE;
        if (FD_ISSET(it->first, &exceptionSet)) rEvent |= IOEvent::EVENT_ERROR;

        if (rEvent != 0) {
            it->second->setREvent(rEvent);
            mIOEvents.push_back(it->second);
        }
    }

    for (auto &ioEvent : mIOEvents) { 
        ioEvent->handleEvent();
    }

    mIOEvents.clear();
}

} // namespace RACLE
