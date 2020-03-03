#include "../../Include/SocketMulticast/SendSocketMulticast.h"

SendSocketMulticast::SendSocketMulticast( const std::string local_addr_,
                                          const std::string group_addr_,
                                          const int& port_)
                                       :  local_addr(local_addr_),
                                          group_addr(group_addr_),
                                          port(port_)
{
}

SendSocketMulticast::~SendSocketMulticast()
{
    close(sockfd);
}

void SendSocketMulticast::Create_Socket()
{
    try
    {
        if( ( sockfd = socket(AF_INET,SOCK_DGRAM,0) ) < 0 )
            throw std::runtime_error("Cannot Create Socket");
        
        bzero(&group,sizeof(group));
        group.sin_family = AF_INET;
        group.sin_port = htons(port);
        group.sin_addr.s_addr = inet_addr(group_addr.c_str());
    }
    catch(const std::runtime_error& r)
    {
        std::cerr << r.what() << ": " << errno << '\n';
    }
}

void SendSocketMulticast::SetSockLoop()
{
    try
    {
        // IP_MULTICAST_LOOP operates on all host? but not only for invidiual socket
        u_char loop = 0;
        if( ( setsockopt(sockfd,
                         IPPROTO_IP,
                         IP_MULTICAST_LOOP, // SET interface for sending
                         (&loop),
                         sizeof(loop)
                         )  ) < 0 )
            throw std::runtime_error("Cannot set REUSEADDR setsockopt()");
    }
    catch(const std::runtime_error& r)
    {
        std::cerr << r.what() << ": " << errno << '\n';
        close(sockfd);
    }
}

void SendSocketMulticast::SetOutInterface()
{
    try
    {
        local.s_addr = htonl(INADDR_ANY);

        if( ( setsockopt(sockfd,
                         IPPROTO_IP,
                         IP_MULTICAST_IF,
                         &local,
                         sizeof(local)
                        ) 
            ) < 0 )
            throw std::runtime_error("Cannot SetLocalInterface");
    }
    catch(const std::runtime_error& r)
    {
        std::cerr << r.what() << ": " << errno << '\n';
        close(sockfd);
    }
}

