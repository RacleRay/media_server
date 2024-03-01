#ifndef __AACSINK__H__
#define __AACSINK__H__

#include "sink.h"
#include "source.h"
#include "uenv.h"

namespace RACLE {

class AACFileSink : public Sink {
  public:
    static AACFileSink *
    createNew(UserEnvironment *env, MediaSource *mediaSource);
    ;

    AACFileSink(
        UserEnvironment *env, MediaSource *mediaSource, int payloadType);
    ~AACFileSink() override;

    std::string getMediaDescription(uint16_t port) override;
    std::string getAttribute() override;

  protected:
    void sendFrame(MediaFrame *frame) override;

  private:
    RtpPacket mRtpPacket;
    uint32_t mSampleRate; // 采样频率
    uint32_t mChannels;   // 通道数
    int mFps;
};

} // namespace RACLE

#endif //!__AACSINK__H__