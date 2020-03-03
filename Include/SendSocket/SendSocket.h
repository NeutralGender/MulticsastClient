#ifndef _SENDSOCKET
#define _SENDSOCKET

#include <iostream>
#include <exception>
#include <string.h>
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


class SendSocket
{
protected:
    int socketfd = 0;
    const std::string s_addr;
    const short port;
    struct sockaddr_in sockaddr;
public:
    SendSocket( const std::string& s_addr_,
                const short port_ );
    ~SendSocket();

    virtual void CreateSocket() = 0;
    virtual void InitSocket() = 0;
    virtual void Connect() = 0;

};

#endif