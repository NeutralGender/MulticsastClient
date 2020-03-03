#ifndef _REDIS_CONNECT
#define _REDIS_CONNECT

#include <iostream>
#include <exception>

#include <hiredis.h>

class RedisConnection
{
private:
    const std::string hostname;
    const int& port;

    redisContext* rconnect;

public:
   RedisConnection(const std::string& hostname_,
                   const int& port_);
   ~RedisConnection();

    void connect();
    friend class RedisCommand; // Must set ALL class as friend,
                               // because of predecloration RedisConnection
};

#endif