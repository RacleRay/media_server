#ifndef __SCHEDULER__H__
#define __SCHEDULER__H__

#include <cstdint>
#include <mutex>
#include <queue>
#include <vector>

#include "timer.h"
#include "event.h"


namespace RACLE {
    
class Scheduler {
public:
    enum PollerType {
        POLLER_SELECT,
        POLLER_POLL,
        POLLER_EPOLL,
    };

public:
    static Scheduler* createNew(PollerType type);

public:
    explicit Scheduler(PollerType type);
    virtual ~Scheduler();

public:
    bool addTriggerEvent(TriggerEvent* event);

    Timer::TimerId addTimedEventRunAfater(TimerEvent* event, Timer::TimerInterval delay);
    Timer::TimerId addTimedEventRunAt(TimerEvent* event, Timer::Timestamp when);
    Timer::TimerId addTimedEventRunEvery(TimerEvent* event, Timer::TimerInterval interval);
    bool removeTimedEvent(Timer::TimerId timerId);
    
    bool addIOEvent(IOEvent* event);
    bool updateIOEvent(IOEvent* event);
    bool removeIOEvent(IOEvent* event);

    void loop();

    [[nodiscard]]
    Poller* poller() const;

    void setTimerManagerReadCallback(EventCallback cb, void* arg);

private:
    void handleTriggerEvents();

private:
    bool mQuit{false};

    Poller* mPoller{nullptr};
    TimerManager* mTimerManager{nullptr};

    // EventCallback mTimerManagerReadCallback{nullptr};
    // void* mTimerManagerArg{nullptr};

    std::vector<TriggerEvent*> mTriggerEvents;
    std::mutex mMutex;
};

}  // namespace RACLE


#endif //!__SCHEDULER__H__