#include "source.h"
#include "log.h"

namespace RACLE {

MediaSource::MediaSource(UserEnvironment *env) : mEnv(env), mFPS(0) {
    for (int i = 0; i < DEFAULT_FRAME_NUM; ++i) {
        mFrameInputQueue.push(&mFrames[i]);
    }

    mTask.setTaskCallback(taskCallback, this);
}

MediaSource::~MediaSource() {
    LOGI("~MediaSource()");
}

//=================================================================================
// 

MediaFrame *MediaSource::getFrameFromOutputQueue() {
    std::lock_guard<std::mutex> lck(mMtx);
    if (mFrameOutputQueue.empty()) { return nullptr; }
    MediaFrame *frame = mFrameOutputQueue.front();
    mFrameOutputQueue.pop();
    return frame;
}

void MediaSource::putFrameToInputQueue(MediaFrame *frame) {
    std::lock_guard<std::mutex> lck(mMtx);

    mFrameInputQueue.push(frame);

    mEnv->threadPool()->addTask(mTask);
}

// invoke handleTask of specific media source.
void MediaSource::taskCallback(void *arg) {
    auto *source = (MediaSource *)arg;
    source->handleTask();
}

} // namespace RACLE
