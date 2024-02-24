#include "buffer.h"
#include "sockets.h"

constexpr int Buffer::initialSize = 1024;
constexpr const char *Buffer::kCRLF = "\r\n";

Buffer::Buffer() : mBufferSize(initialSize), mReadIndex(0), mWriteIndex(0) {
    mBuffer = (char *)malloc(mBufferSize);
}

Buffer::~Buffer() {
    free(mBuffer);
}

int Buffer::read(int fd) {
    char extrabuf[65536];
    const int writable = writableBytes();
    const int n = (int)::recv(fd, extrabuf, sizeof(extrabuf), 0);
    if (n <= 0) {
        return -1;
    }
    
    if (n <= writable) {
        std::copy(extrabuf, extrabuf + n, beginWrite());
        mWriteIndex += n;

    } else {
        std::copy(extrabuf, extrabuf + writable, beginWrite());
        mWriteIndex += writable;
        append(extrabuf + writable, n - writable);
    }

    return n;
}

int Buffer::write(int fd) {
    return (int)sockets::write(fd, peek(), readableBytes());
}