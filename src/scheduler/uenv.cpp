#include "uenv.h"

namespace RACLE {

UserEnvironment *
UserEnvironment::createNew(Scheduler *scheduler, ThreadPool *threadPool) {
    return new UserEnvironment(scheduler, threadPool);
}

UserEnvironment::UserEnvironment(Scheduler *scheduler, ThreadPool *threadPool)
    : mScheduler(scheduler), mThreadPool(threadPool) {
}

Scheduler *UserEnvironment::scheduler() {
    return mScheduler;
}

ThreadPool *UserEnvironment::threadPool() {
    return mThreadPool;
}

} // namespace RACLE
