#ifndef __H264SINK__H__
#define __H264SINK__H__

#include <cstdint>

#include "sink.h"

namespace RACLE {

class H264FileSink : public Sink {
  public:
    static H264FileSink *
    createNew(UserEnvironment *env, MediaSource *mediaSource);

    H264FileSink(UserEnvironment *env, MediaSource *mediaSource);
    ~H264FileSink() override;
    
    std::string getMediaDescription(uint16_t port) override;
    std::string getAttribute() override;
    void sendFrame(MediaFrame *frame) override;

  private:
    RtpPacket mRtpPacket;
    int mClockRate;
    int mFps;
};

} // namespace RACLE

#endif //!__H264SINK__H__