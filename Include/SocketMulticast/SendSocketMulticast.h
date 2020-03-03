#ifndef _SEND_SOCKET
#define _SEND_SOCKET

#include <iostream>
#include <string.h>
#include <exception>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

class SendSocketMulticast
{
private:
    int sockfd = -1;
    struct in_addr local;
    struct sockaddr_in group;

    const std::string local_addr;
    const std::string group_addr;
    const int& port;

public:
    SendSocketMulticast(const std::string local_addr_,
                        const std::string group_addr_,
                        const int& port_);
    ~SendSocketMulticast();

    void Create_Socket();
    void SetSockLoop();
    void SetOutInterface();

    friend class MulticastServer;
};


#endif