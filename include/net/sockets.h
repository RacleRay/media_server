#ifndef __SOCKETS__H__
#define __SOCKETS__H__

#include <string>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

namespace sockets {

int createTcpSock(); // 默认创建非阻塞的tcp描述符
int createUdpSock(); // 默认创建非阻塞的udp描述符

bool bind(int sockfd, const std::string& ip, uint16_t port);
bool listen(int sockfd, int backlog);
int accept(int sockfd);
void close(int sockfd);
bool connect(int sockfd, const std::string& ip, uint16_t port, int timeout);

void ignoreSigPipeOnSocket(int socketfd);

ssize_t write(int sockfd, const void *buf, int size); // tcp
ssize_t sendto(
    int sockfd,
    const void *buf,
    int len,
    const struct sockaddr *destAddr);       // udp

int setNonBlock(int sockfd);                // 设置非阻塞模式
int setBlock(int sockfd, int writeTimeout); // 设置阻塞模式

void setReuseAddr(int sockfd, int on);
void setReusePort(int sockfd);
void setNonBlockAndCloseOnExec(int sockfd);

void setNoDelay(int sockfd);
void setKeepAlive(int sockfd);
void setNoSigpipe(int sockfd);
void setSendBufSize(int sockfd, int size);
void setRecvBufSize(int sockfd, int size);

std::string getPeerIp(int sockfd);
int16_t getPeerPort(int sockfd);
int getPeerAddr(int sockfd, struct sockaddr_in *addr);
std::string getLocalIp();

} // namespace sockets

#endif //!__SOCKETS__H__