#include <sys/eventfd.h>

#include "scheduler.h"
#include "selector.h"
#include "timer.h"


namespace RACLE {
    
Scheduler::Scheduler(PollerType type)
{
    switch (type) {
        case PollerType::POLLER_SELECT: {
            // get new select
            // mPoller = 
        }
        default: {
            exit(-1);
            break;
        }
    }
    mTimerManager = TimerManager::createNew(this);
}


Scheduler* Scheduler::createNew(PollerType type)
{
    // if (type != POLLER_SELECT && type != POLLER_POLL && type != POLLER_EPOLL)
    //     return nullptr;

    if (type == PollerType::POLLER_SELECT) {
        return nullptr;
    }

    return new Scheduler(type);
}


Scheduler::~Scheduler() {
    delete mTimerManager;
    delete mPoller;
}


bool Scheduler::addTriggerEvent(TriggerEvent* event)
{
    mTriggerEvents.push_back(event);

    return true;
}

Timer::TimerId Scheduler::addTimedEventRunAfater(TimerEvent* event, Timer::TimerInterval delay)
{
    Timer::Timestamp timestamp = Timer::getCurTime();
    timestamp += delay;

    return mTimerManager->addTimer(event, timestamp, 0);
}

Timer::TimerId Scheduler::addTimedEventRunAt(TimerEvent* event, Timer::Timestamp when)
{
    return mTimerManager->addTimer(event, when, 0);
}

Timer::TimerId Scheduler::addTimedEventRunEvery(TimerEvent* event, Timer::TimerInterval interval)
{
    Timer::Timestamp timestamp = Timer::getCurTime();
    timestamp += interval;

    return mTimerManager->addTimer(event, timestamp, interval);
}

bool Scheduler::removeTimedEvent(Timer::TimerId timerId)
{
    return mTimerManager->removeTimer(timerId);
}

bool Scheduler::addIOEvent(IOEvent* event)
{
    return mPoller->addIOEvent(event);
}

bool Scheduler::updateIOEvent(IOEvent* event)
{
    return mPoller->updateIOEvent(event);
}

bool Scheduler::removeIOEvent(IOEvent* event)
{
    return mPoller->removeIOEvent(event);
}

void Scheduler::loop() {

#ifdef WIN32
    // windows 下模拟select定时器效果
    std::thread([](Scheduler* sch) {
        while (!sch->mQuit) {
            if (sch->mTimerManagerReadCallback) {
                sch->mTimerManagerReadCallback(sch->mTimerManagerArg);
            }
        }
        }, this).detach();
#endif // WIN32
    // linux 下使用 select 定时
    while (!mQuit) {
        handleTriggerEvents();
        mPoller->handleEvent();
    }
}

void Scheduler::handleTriggerEvents()
{
    if (!mTriggerEvents.empty())
    {
        for (auto it = mTriggerEvents.begin(); it != mTriggerEvents.end(); ++it)
        {
            (*it)->handleEvent();
        }

        mTriggerEvents.clear();
    }
}

Poller* Scheduler::poller() const {
    return mPoller;
}

// void Scheduler::setTimerManagerReadCallback(EventCallback cb, void* arg)
// {
//     mTimerManagerReadCallback = std::move(cb);
//     mTimerManagerArg = arg;
// }

}  // namespace RACLE

