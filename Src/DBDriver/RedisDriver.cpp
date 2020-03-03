#include "../../Include/DBDriver/RedisDriver.h"

RedisDriver::RedisDriver(const std::string& hostname_,
                           const int& port_) 
                         : rconnection(hostname_,port_)
{
}

RedisDriver::~RedisDriver()
{
}

void RedisDriver::ConnectToDb()
{
    rconnection.connect();
}

void RedisDriver::Write(std::array<char,512> arr)
{
    r_read.Write(&rconnection, arr);
}

void RedisDriver::Read()
{
    r_read.Read(&rconnection, "key");
}

void RedisDriver::GetAllData(std::map<std::pair<std::string,std::string>,
                                      std::string>& key_value)
{
    r_read.GetAllData(&rconnection,key_value);
}

void RedisDriver::PipelineWrite(std::map<std::pair<std::string,std::string>,
                                         std::string>& key_value)
{
    r_read.PipelineWrite(&rconnection, key_value);
}

void RedisDriver::Authentication()
{
    r_read.Authentication(&rconnection);
}