#ifndef __UENV__H__
#define __UENV__H__

#include "scheduler.h"
#include "threadpool.h"


namespace RACLE {

class UserEnvironment {
  public:
    static UserEnvironment *createNew(Scheduler *scheduler, ThreadPool *threadPool);

  public:
    UserEnvironment(Scheduler *scheduler, ThreadPool *threadPool);
    ~UserEnvironment() = default;

    Scheduler *scheduler();
    ThreadPool *threadPool();

  private:
    Scheduler *mScheduler;
    ThreadPool *mThreadPool;
};

} // namespace RACLE

#endif //!__UENV__H__