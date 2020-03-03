#include "../Include/RedisConnection.h"

RedisConnection::RedisConnection(const std::string& hostname_,
                                 const int& port_)
                               : hostname(hostname_),
                                 port(port_)
{

}

RedisConnection::~RedisConnection()
{
    redisFree(rconnect);
}

void RedisConnection::connect()
{
    try
    {
        rconnect = redisConnect(hostname.c_str(), port);

        if(rconnect == NULL || rconnect->err)
            throw std::exception();
        else
            std::cout << "Connected!\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}