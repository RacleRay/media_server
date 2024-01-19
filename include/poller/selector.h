#ifndef __SELECTOR__H__
#define __SELECTOR__H__

#include <vector>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include "poller.h"

namespace RACLE {

class SelectPoller : public Poller {
  public:
    SelectPoller();
    ~SelectPoller() override = default;

    static SelectPoller *createNew();

    bool addIOEvent(IOEvent *event) override;
    bool updateIOEvent(IOEvent *event) override;
    bool removeIOEvent(IOEvent *event) override;
    void handleEvent() override;

  private:
    fd_set mReadSet;
    fd_set mWriteSet;
    fd_set mExceptionSet;
    int mMaxNumSockets;
    std::vector<IOEvent *> mIOEvents;  // 存储临时活跃的IO事件对象
};

} // namespace RACLE

#endif //!__SELECTOR__H__