#include <ctime>
#include <sys/timerfd.h>

#include <chrono>

#include "event.h"
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
}




}  // namespace RACLE