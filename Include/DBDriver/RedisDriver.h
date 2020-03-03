#ifndef _REDIS_DRIVER
#define _REDIS_DRIVER

#include "DBDriver.h"
#include "RedisConnection.h"
#include "RedisCommand.h"

#include <array>
#include <map>

class Driver;

class RedisDriver : public DBDriver
{
private:
    RedisConnection rconnection;
    RedisCommand r_read;

public:
    RedisDriver(const std::string& hostname,
                const int& port);
    virtual ~RedisDriver();

    virtual void ConnectToDb() override;
    virtual void Read() override;
    virtual void GetAllData(std::map<std::pair<std::string,std::string>,
                            std::string>&) override;
    virtual void PipelineWrite(std::map<std::pair<std::string,std::string>,
                               std::string>&) override;
    virtual void Authentication() override;
    virtual void Write(std::array<char,512>) override;

    friend class Driver;
};

#endif