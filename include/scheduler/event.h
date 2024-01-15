#ifndef __EVENT__H__
#define __EVENT__H__

#include <functional>

#include "log.h"

namespace RACLE {

using EventCallback = std::function<void(void*)>;

//=================================================================================
// TriggerEvent

class TriggerEvent {
public:
    static TriggerEvent* createNew(void* arg) {
        return new TriggerEvent(arg);
    }
    static TriggerEvent* createNew() {
        return new TriggerEvent(nullptr);
    }

public:
    explicit TriggerEvent(void* arg) : mArgs(arg) {
        LOGI("TriggerEvent::TriggerEvent()");
    }
    ~TriggerEvent() {
        LOGI("TriggerEvent::~TriggerEvent()");
    }

    void setArg(void* arg) { 
        mArgs = arg; 
    }
    
    void setTriggerCallback(EventCallback callback) { 
        mTriggerCallback = std::move(callback); 
    }

    void handleEvent() {
        if (mTriggerCallback) {
            mTriggerCallback(mArgs);
        }
    }

private:
    void* mArgs = nullptr;
    EventCallback mTriggerCallback{nullptr};
};


//=================================================================================
// 

class TimerEvent {
public:
    static TimerEvent* createNew(void* arg) {
        return new TimerEvent(arg);
    }
    static TimerEvent* createNew() {
        return new TimerEvent(nullptr);
    }

public:
    explicit TimerEvent(void* arg) 
        : mArg(arg)
    {
        LOGI("TimerEvent::TimerEvent()");
    }
    ~TimerEvent() {
        LOGI("TimerEvent::~TimerEvent()");
    }

    void setArg(void* arg) { mArg = arg; }
    void setTimeoutCallback(EventCallback cb) { mTimeoutCallback = std::move(cb); }
    bool handleEvent() {
        if (mIsStop) {
            return true;
        }

        if (mTimeoutCallback) {
            mTimeoutCallback(mArg);
        }

        return false;
    }

    void stop() {
        mIsStop = true;
    }

private:
    void* mArg{nullptr};
    EventCallback mTimeoutCallback{nullptr};
    bool mIsStop{false};
};


//=================================================================================
// IOEvent

class IOEvent{
public:
    enum IOEventType
    {
        EVENT_NONE = 0,
        EVENT_READ = 1,
        EVENT_WRITE = 2,
        EVENT_ERROR = 4,
    };
    
    static IOEvent* createNew(int fd, void* arg) {
        if (fd < 0) {
            return nullptr;
        }
        return new IOEvent(fd, arg);
    }
    static IOEvent* createNew(int fd) {
        return createNew(fd, nullptr);
    }

    IOEvent(int fd, void* arg)
        : mFd(fd), mArg(arg)
    {
        LOGI("IOEvent::IOEvent() : fd = [%d]", fd);
    }
    ~IOEvent() {
        LOGI("IOEvent::~IOEvent() : fd = [%d]", mFd);
    }

    [[nodiscard]]
    int getFd() const { return mFd; }
    [[nodiscard]]
    int getEvent() const { return mEvent; }
    void setREvent(int event) { mREvent = event; }
    void setArg(void* arg) { mArg = arg; }

    void setReadCallback(EventCallback cb) { mReadCallback = std::move(cb); };
    void setWriteCallback(EventCallback cb) { mWriteCallback = std::move(cb); };
    void setErrorCallback(EventCallback cb) { mErrorCallback = std::move(cb); };

    void enableReadHandling() { mEvent |= EVENT_READ; }
    void enableWriteHandling() { mEvent |= EVENT_WRITE; }
    void enableErrorHandling() { mEvent |= EVENT_ERROR; }
    void disableReadeHandling() { mEvent &= ~EVENT_READ; }
    void disableWriteHandling() { mEvent &= ~EVENT_WRITE; }
    void disableErrorHandling() { mEvent &= ~EVENT_ERROR; }

    [[nodiscard]]
    bool isNoneHandling() const { return mEvent == EVENT_NONE; }
    [[nodiscard]]
    bool isReadHandling() const { return (mEvent & EVENT_READ) != 0; }
    [[nodiscard]]
    bool isWriteHandling() const { return (mEvent & EVENT_WRITE) != 0; }
    [[nodiscard]]
    bool isErrorHandling() const { return (mEvent & EVENT_ERROR) != 0; };

    void handleEvent() {
        if (mReadCallback && (mREvent & EVENT_READ)) {
            mReadCallback(mArg);
        }
        if (mWriteCallback && (mREvent & EVENT_WRITE)) {
            mWriteCallback(mArg);
        }
        if (mErrorCallback && (mREvent & EVENT_ERROR)) {
            mErrorCallback(mArg);
        }
    }

private:
    int mFd;
    void* mArg{nullptr};
    int mEvent{EVENT_NONE};
    int mREvent{EVENT_NONE};
    EventCallback mReadCallback{nullptr};
    EventCallback mWriteCallback{nullptr};
    EventCallback mErrorCallback{nullptr};
};


}  // namespace RACLE

#endif  //!__EVENT__H__