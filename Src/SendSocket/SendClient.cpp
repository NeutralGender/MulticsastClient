#include "../../Include/SendSocket/SendClient.h"

SendClient::SendClient( const std::string& s_addr_,
                        const short port_ )
                      : SendSocket(s_addr_, port_)
{
}

SendClient::~SendClient()
{
}

void SendClient::CreateSocket()
{
    try
    {
        socketfd = socket( AF_INET, SOCK_STREAM, 0 );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void SendClient::InitSocket()
{
    try
    {
        bzero( &sockaddr, sizeof(sockaddr) );
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_port = htons(port);
        sockaddr.sin_addr.s_addr = inet_addr( s_addr.c_str() );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void SendClient::Connect()
{
    try
    {
        int check = 0;
        if( ( check = connect( socketfd, 
                               (struct sockaddr*)&sockaddr, 
                               sizeof(sockaddr) 
                             ) 
            ) < 0
          )
          throw std::runtime_error(" Connect() Error \n");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}
