#ifndef __TIMER__H__
#define __TIMER__H__

#include <cstdint>
#include <map>


namespace RACLE {

class Scheduler;
class IOEvent;
class Poller;
class TimerEvent;


//=================================================================================
// Timer

class Timer {
public:
    using TimerId = uint32_t;
    using Timestamp = int64_t;
    using TimerInterval = uint32_t;

public:
    ~Timer() = default;

public:
    static Timestamp getCurTime();       // 获取系统启动以来毫秒数
    static Timestamp getCurTimestamp();  // 获取毫秒级时间戳 

private:
    Timer(TimerEvent* event, Timestamp timestamp, TimerInterval interval, TimerId timerId);

    bool handleEvent();

private:

    TimerEvent* mTimerEvent {nullptr};
    Timestamp mTimestamp {0};
    TimerInterval mInterval {0};
    TimerId mTimerId {0};

    bool mRepeat{false};
};


//=================================================================================
// TimerManager

class TimerManager {
public:
    explicit TimerManager(Scheduler* scheduler);
    ~TimerManager();

    Timer::TimerId addTimer(Timer::Timestamp Timestamp, Timer::TimerInterval interval);
    void removeTimer(Timer::TimerId timerId);

public:
    static TimerManager* createNew(Scheduler* scheduler);

private:
    static void readCallback(void* arg);

private:
    void handleRead();
    void modifyTimeout();

private:
    Poller* mPoller {nullptr};

    Timer::TimerId mLastTimerId {0};

    int mTimerFd {0};
    IOEvent* mTimerIOEvent {nullptr};

    std::map<Timer::TimerId, Timer> mTimers;
    std::multimap<Timer::Timestamp, Timer> mEvents;
};


}  // namespace RACLE


#endif //!__TIMER__H__