#ifndef __AACSOURCE__H__
#define __AACSOURCE__H__

#include <string>

#include "source.h"

namespace RACLE {

class AACFileMeidaSource : public MediaSource {
  public:
    static AACFileMeidaSource *
    createNew(UserEnvironment *env, const std::string &file);

    AACFileMeidaSource(UserEnvironment *env, const std::string &file);
    ~AACFileMeidaSource() override;

  protected:
    void handleTask() override;

  private:
    struct AdtsHeader {
        unsigned int syncword; // 12 bit 同步字 '1111 1111 1111'，一个ADTS帧的开始
        unsigned int id;       // 1 bit MPEG 标示符: 0 is MPEG-4，1 is MPEG-2
        unsigned int layer;            // 2 bit 总是'00'
        unsigned int protectionAbsent; // 1 bit 1表示没有crc，0表示有crc
        unsigned int profile;          // 1 bit 表示使用哪个级别的AAC
        unsigned int samplingFreqIndex; // 4 bit 表示使用的采样频率
        unsigned int privateBit;        // 1 bit
        unsigned int channelCfg;        // 3 bit 表示声道数
        unsigned int originalCopy;      // 1 bit
        unsigned int home;              // 1 bit

        unsigned int copyrightIdentificationBit;   // 1 bit
        unsigned int copyrightIdentificationStart; // 1 bit
        unsigned int aacFrameLength; // 13 bit 一个ADTS帧的长度包括ADTS头和AAC原始流
        unsigned int adtsBufferFullness; // 11 bit 0x7FF 说明是码率可变的码流

        /* number_of_raw_data_blocks_in_frame
         * 表示ADTS帧中有 number_of_raw_data_blocks_in_frame + 1 个AAC原始帧
         * 一个AAC原始帧包含一段时间内1024个采样及相关数据
         */
        unsigned int numberOfRawDataBlockInFrame; // 2 bit
    };

    bool parseAdtsHeader(uint8_t *in, struct AdtsHeader *res);
    int getFrameFromAACFile(uint8_t *buf, int size);

  private:
    FILE *mFile;
    struct AdtsHeader mAdtsHeader;
};

} // namespace RACLE

#endif //!__AACSOURCE__H__