#ifndef __BUFFER__H__
#define __BUFFER__H__

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>


class Buffer {
  public:
    static const int initialSize;

    explicit Buffer();
    ~Buffer();

    [[nodiscard]] int readableBytes() const { return mWriteIndex - mReadIndex; }

    [[nodiscard]] int writableBytes() const {
        return mBufferSize - mWriteIndex;
    }

    [[nodiscard]] int prependableBytes() const { return mReadIndex; }

    char *peek() { return begin() + mReadIndex; }

    [[nodiscard]] const char *peek() const { return begin() + mReadIndex; }

    [[nodiscard]] const char *findCRLF() const {
        const char *crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
        return crlf == beginWrite() ? nullptr : crlf;
    }

    const char *findCRLF(const char *start) const {
        assert(peek() <= start);
        assert(start <= beginWrite());
        const char *crlf = std::search(start, beginWrite(), kCRLF, kCRLF + 2);
        return crlf == beginWrite() ? nullptr : crlf;
    }

    [[nodiscard]] const char *findLastCrlf() const {
        const char *crlf =
            std::find_end(peek(), beginWrite(), kCRLF, kCRLF + 2);
        return crlf == beginWrite() ? nullptr : crlf;
    }

    void retrieveReadZero() {
        mReadIndex = 0;
    }

    void retrieve(int len) {
        assert(len <= readableBytes());
        if (len < readableBytes()) {
            mReadIndex += len;
        } else {
            retrieveAll();
        }
    }

    void retrieveUntil(const char *end) {
        assert(peek() <= end);
        assert(end <= beginWrite());
        retrieve((int)(end - peek()));
    }

    void retrieveAll()
    {
        mReadIndex = 0;
        mWriteIndex = 0;
    }

    char *beginWrite() { return begin() + mWriteIndex; }

    [[nodiscard]] const char *beginWrite() const {
        return begin() + mWriteIndex;
    }

    void unwrite(int len) {
        assert(len <= readableBytes());
        mWriteIndex -= len;
    }

    void ensureWritableBytes(int len) {
        if (writableBytes() < len) { makeSpace(len); }
        assert(writableBytes() >= len);
    }

    void makeSpace(int len) {
        if (writableBytes() + prependableBytes() < len) {
            mBufferSize = mWriteIndex + len;
            mBuffer = (char *)realloc(mBuffer, mBufferSize);
        } else // enough
        {
            int readable = readableBytes();
            std::copy(begin() + mReadIndex, begin() + mWriteIndex, begin());
            mReadIndex = 0;
            mWriteIndex = mReadIndex + readable;
            assert(readable == readableBytes());
        }
    }

    void append(const char *data, int len) {
        ensureWritableBytes(len);
        std::copy(data, data + len, beginWrite());
        assert(len <= writableBytes());
        mWriteIndex += len;
    }

    void append(const void *data, int len) {
        append((const char *)(data), len);
    }

    int read(int fd);
    int write(int fd);

  private:
    char *begin() { return mBuffer; }

    [[nodiscard]] const char *begin() const { return mBuffer; }

  private:
    char *mBuffer;
    int mBufferSize;
    int mReadIndex;
    int mWriteIndex;

    static const char *const kCRLF;
};

#endif //!__BUFFER__H__