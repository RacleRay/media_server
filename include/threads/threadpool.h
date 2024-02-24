#ifndef __THREADPOOL__H__
#define __THREADPOOL__H__

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class Thread {
  public:
    virtual ~Thread();

    bool start(void *arg);
    bool detach();
    bool join();

  protected:
    Thread();

    virtual void run(void *arg) = 0;

  private:
    static void *threadRun(void *);

  private:
    void *mArg{nullptr};
    bool mIsStart{false};
    bool mIsDetach{false};
    std::thread mThreadId;
};

//=================================================================================
// 低性能，未优化，极其普通线程池实现

class ThreadPool {
  public:
    class Task {
      public:
        // typedef void (*TaskCallback)(void *);
        using TaskCallback = void (*)(void *);

        Task() = default;

        void setTaskCallback(TaskCallback cb, void *arg) {
            mTaskCallback = cb;
            mArg = arg;
        }

        void handle() {
            if (mTaskCallback) { 
                mTaskCallback(mArg);
            }
        }

        Task& operator=(const Task &task) {
            if (this == &task) { 
                return *this;
            }
            this->mTaskCallback = task.mTaskCallback;
            this->mArg = task.mArg;
            return *this;
        }

      private:
        TaskCallback mTaskCallback{nullptr};
        void *mArg{nullptr};
    };

    explicit ThreadPool(int num);
    ~ThreadPool();

    void addTask(Task &task);

  public:
    static ThreadPool *createNew(int num);

  private:
    void loop();

    class MThread : public Thread {
      protected:
        void run(void *arg) override;
    };

    void createThreads();
    void cancelThreads();

  private:
    std::queue<Task> mTaskQueue;
    std::mutex mMtx;
    std::condition_variable mCon;

    std::vector<MThread> mThreads;
    bool mQuit;
};

#endif //!__THREADPOOL__H__