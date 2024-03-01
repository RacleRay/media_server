#ifndef __H264SOURCE__H__
#define __H264SOURCE__H__

#include <string>

#include "source.h"

namespace RACLE {

class H264FileMediaSource : public MediaSource {
  public:
    static H264FileMediaSource *
    createNew(UserEnvironment *env, const std::string &file);

    H264FileMediaSource(UserEnvironment *env, const std::string &file);
    ~H264FileMediaSource() override;

  protected:
    void handleTask() override;

  private:
    int getFrameFromH264File(uint8_t *frame, int size);

  private:
    FILE *mFile;
};

} // namespace RACLE

#endif //!__H264SOURCE__H__