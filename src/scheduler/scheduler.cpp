#include <sys/eventfd.h>

#include "scheduler.h"
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

}  // namespace RACLE

