#include "address.h"

IPv4Address::IPv4Address(std::string ip, uint16_t port)
    : mIp(std::move(ip)), mPort(port) {
    mAddr.sin_family = AF_INET;
    mAddr.sin_addr.s_addr = inet_addr(mIp.c_str());
    mAddr.sin_port = htons(mPort);
}

void IPv4Address::setAddr(std::string ip, uint16_t port) {
    mIp = std::move(ip);
    mPort = port;
    mAddr.sin_family = AF_INET;
    mAddr.sin_addr.s_addr = inet_addr(mIp.c_str());
    mAddr.sin_port = htons(port);
}

std::string IPv4Address::getIP() const {
    return mIp;
}

uint16_t IPv4Address::getPort() const {
    return mPort;
}

struct sockaddr *IPv4Address::getAddr() {
    return (struct sockaddr *)&mAddr;
}
