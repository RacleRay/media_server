#include <ctime>
#include <sys/timerfd.h>

#include <chrono>

#include "event.h"
#include "poller.h"
#include "scheduler.h"
#include "timer.h"


namespace RACLE {

Timer::Timer(TimerEvent* event, Timestamp timestamp, TimerInterval interval, TimerId timerId)
    : mTimerEvent(event), mTimestamp(timestamp), mInterval(interval), mTimerId(timerId) 
{
    if (interval > 0) {
        mRepeat = true;
    }
    mRepeat = false;
}


Timer::Timestamp Timer::getCurTime()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec * 1000 + now.tv_nsec / 1000000);   // to ms

    // auto now = std::chrono::system_clock::now().time_since_epoch().count();
    // return now / 1000000;
}

Timer::Timestamp Timer::getCurTimestamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count(); 
}

bool Timer::handleEvent()
{
    if (!mTimerEvent) [[unlikely]] {
        return false;
    }
    return mTimerEvent->handleEvent();
}


//=================================================================================
// 

TimerManager::TimerManager(Scheduler* scheduler)
    : mPoller(scheduler->poller())
{
    mTimerFd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

    if (mTimerFd < 0) {
        LOGE("TimerManager::TimerManager : create timerfd error.");
        return ;
    }

    LOGI("TimerManager::TimerManager : create timerfd [%d].", mTimerFd);

    mTimerIOEvent = IOEvent::createNew(mTimerFd, this);
    mTimerIOEvent->setReadCallback(readCallback);
    mTimerIOEvent->enableReadHandling();

    mPoller->addIOEvent(mTimerIOEvent);
}


TimerManager* TimerManager::createNew(Scheduler* scheduler)
{
    if (!scheduler) {
        return nullptr;
    }
    return new TimerManager(scheduler);
}


Timer::TimerId TimerManager::addTimer(TimerEvent* event, Timer::Timestamp timestamp, Timer::TimerInterval interval)
{
    mLastTimerId++;
    Timer timer(event, timestamp, interval, mLastTimerId);

    mTimers.insert(std::move(std::make_pair(mLastTimerId, timer)));
    mEvents.insert(std::move(std::make_pair(timestamp, timer)));

    return mLastTimerId;
}


bool TimerManager::removeTimer(Timer::TimerId timerId)
{
    auto it = mTimers.find(timerId);
    if (it != mTimers.end()) {
        mTimers.erase(timerId);
    }   
    return true;
}


void TimerManager::readCallback(void* arg)
{
    auto* timerManager = static_cast<TimerManager*>(arg);
    timerManager->handleRead();   
}


void TimerManager::handleRead() {
    auto timestamp = Timer::getCurTime();
    
    if (!mTimers.empty() && !mEvents.empty()) {
        auto it = mEvents.begin();
        auto timer = it->second;
        int expire = timer.mTimestamp - timestamp;

        if (timestamp > timer.mTimestamp || expire <= 0) {
            bool timerEventIsStop = timer.handleEvent();
            mEvents.erase(it);
            if (timer.mRepeat) {
                if (timerEventIsStop) {
                    mTimers.erase(timer.mTimerId);
                }else {
                    timer.mTimestamp = timestamp + timer.mInterval;
                    mEvents.insert(std::make_pair(timer.mTimestamp, timer));
                }
            } else {
                mTimers.erase(timer.mTimerId);
            }
        }
    }
}


}  // namespace RACLE