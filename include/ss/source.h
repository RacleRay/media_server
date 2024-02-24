#ifndef __MEDIASOURCE__H__
#define __MEDIASOURCE__H__

#include <cstdint>
#include <mutex>
#include <queue>

#include "threadpool.h"
#include "uenv.h"

// #define FRAME_MAX_SIZE    (1024 * 200)
#define DEFAULT_FRAME_NUM 4

constexpr uint64_t FRAME_MAX_SIZE = static_cast<uint64_t>(1024 * 200);

namespace RACLE {

class MediaFrame {
  public:
    MediaFrame() : mData(new uint8_t[FRAME_MAX_SIZE]) {}
    ~MediaFrame() { delete[] mData; }

    uint8_t *mData;             // 容器
    uint8_t *mDataRef{nullptr}; // 引用容器
    int mSize{0};
};

/// <summary>
/// AAC data 和 H264 data 的基类
/// </summary>
class MediaSource {
  public:
    explicit MediaSource(UserEnvironment *env);
    virtual ~MediaSource();

    MediaFrame *getFrameFromOutputQueue();        // 从输出队列获取帧
    void putFrameToInputQueue(MediaFrame *frame); // 把帧送入输入队列

    [[nodiscard]] int getFps() const { return mFPS; }
    std::string getSourceName() { return mSourceName; }

  private:
    static void taskCallback(void *arg);

  protected:
    virtual void handleTask() = 0;
    void setFPS(int fps) { mFPS = fps; }

  protected:
    UserEnvironment *mEnv;
    MediaFrame mFrames[DEFAULT_FRAME_NUM];

    std::queue<MediaFrame *> mFrameInputQueue;
    std::queue<MediaFrame *> mFrameOutputQueue;

    std::mutex mMtx;
    ThreadPool::Task mTask;

    int mFPS;
    std::string mSourceName;
};

} // namespace RACLE

#endif //!__MEDIASOURCE__H__