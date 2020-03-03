#ifndef _REDIS_COMMAND
#define _REDIS_COMMAND

#include <iostream>
#include <string>
#include <array>
#include <map>

#include "hiredis.h"

class RedisConnection;
class RedisDriver;

///
#include <boost/algorithm/hex.hpp>
///

class RedisCommand
{
private:
    redisReply* reply;
public:
    RedisCommand();
    ~RedisCommand();

    void Write(RedisConnection* rconnection,
               std::array<char,512>arr);

    void Read(RedisConnection* rconnection,
              std::string_view key);

    void PipelineWrite(RedisConnection* rconnection,
                       std::map<std::pair<std::string,std::string>,
                       std::string>& key_value);
    
    void GetAllData(RedisConnection* rconnection,
                    std::map<std::pair<std::string,std::string>,
                    std::string>& key_value);

    void Authentication(RedisConnection* rconnection);


    friend class RedisDriver;
};

#endif