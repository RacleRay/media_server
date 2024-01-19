#include "threadpool.h"

//=================================================================================
//

Thread::Thread() = default;

Thread::~Thread() {
    if (mIsStart && !mIsDetach) { detach(); }
}

bool Thread::start(void *arg) {
    mArg = arg;
    mThreadId = std::thread(&Thread::threadRun, this);
    mIsStart = true;
    return true;
}

bool Thread::detach() {
    if (!mIsStart) { return false; }
    if (mIsDetach) { return true; }
    mThreadId.detach();
    mIsDetach = true;
    return true;
}

bool Thread::join() {
    if (!mIsStart || mIsDetach) { return false; }
    mThreadId.join();
    return true;
}

void *Thread::threadRun(void *arg) {
    auto *thread = (Thread *)arg;
    thread->run(thread->mArg);
    return nullptr;
}

//=================================================================================
// 低性能，未优化，及其普通线程池实现

ThreadPool *ThreadPool::createNew(int num) {
    return new ThreadPool(num);
}

ThreadPool::ThreadPool(int num) : mThreads(num), mQuit(false) {
    createThreads();
}

ThreadPool::~ThreadPool() {
    cancelThreads();
}

void ThreadPool::addTask(ThreadPool::Task &task) {
    std::unique_lock<std::mutex> lck(mMtx);
    mTaskQueue.push(task);
    mCon.notify_one();
}

void ThreadPool::loop() {
    while (!mQuit) {
        std::unique_lock<std::mutex> lck(mMtx);
        if (mTaskQueue.empty()) { mCon.wait(lck); }

        if (mTaskQueue.empty()) { continue; }

        Task task = mTaskQueue.front();
        mTaskQueue.pop();

        task.handle();
    }
}

void ThreadPool::createThreads() {
    std::unique_lock<std::mutex> lck(mMtx);
    for (auto &mThread : mThreads) { 
        mThread.start(this);
    }
}

void ThreadPool::cancelThreads() {
    std::unique_lock<std::mutex> lck(mMtx);
    mQuit = true;
    mCon.notify_all();
    for (auto &mThread : mThreads) {
        mThread.join();
    }

    mThreads.clear();
}

void ThreadPool::MThread::run(void *arg) {
    auto *threadPool = (ThreadPool *)arg;
    threadPool->loop();
}
