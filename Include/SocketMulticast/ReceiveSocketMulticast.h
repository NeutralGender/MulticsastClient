#ifndef _RECEIVE_SOCKET
#define _RECEIVE_SOCKET

#include <iostream>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

class ReceiveSocketMulticast
{
private:
    int sockfd = -1;
    struct sockaddr_in local; // local interface
    struct ip_mreq group; // kernel join multicast

    const std::string local_addr;
    const std::string group_addr;
    const int& port;

public:
    ReceiveSocketMulticast( const std::string local_addr_,
                            const std::string group_addr_,
                            const int& port_);
    virtual ~ReceiveSocketMulticast();

    void Create_Socket();
    void SetReuseAddr();
    void Bind();
    void JoinToGroup();

    friend class MulticastServer;
};


#endif