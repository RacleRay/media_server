#ifndef __ADDRESS__H__
#define __ADDRESS__H__

#include <cstdint>
#include <string>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>


class IPv4Address {
public:
    IPv4Address() = default;
    IPv4Address(std::string ip, uint16_t port);

    void setAddr(std::string ip, uint16_t port);
    [[nodiscard]]
    std::string getIP() const;
    [[nodiscard]]
    uint16_t getPort() const;

    struct sockaddr* getAddr();

private:
    std::string mIp;
    uint16_t mPort;
    struct sockaddr_in mAddr;
};






#endif //!__ADDRESS__H__