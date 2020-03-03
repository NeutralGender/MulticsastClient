#include "../Include/RedisCommand.h"
#include "../Include/RedisConnection.h" // Must be included, because of incomplete class
                                        // (class in another file)

RedisCommand::RedisCommand()
{

}

RedisCommand::~RedisCommand()
{
    freeReplyObject(reply);
}

void RedisCommand::PipelineWrite( RedisConnection* rconnection,
                                  std::map< std::pair<std::string,std::string>,
                                            std::string
                                          >& key_value
                                )
{
    int appednState = -1;

    try
    {
        for(auto it = key_value.cbegin(); it != key_value.cend(); it++ )
        {
            appednState = (redisAppendCommand(rconnection->rconnect,
                                              "HSET %b %b %b",
                                              it->first.first.c_str(),  
                                                (size_t) strlen (it->first.first.data()),
                                             it->first.second.data(), 
                                                (size_t) sizeof (it->first.second.data()),
                                             it->second.data(),       
                                                (size_t) sizeof (it->second.data())
                                            )
                        );
                      
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << " : " << errno << '\n';
    }
    
    try
    {
        // Retrieve the reply
        for(auto it = key_value.cbegin(); it != key_value.cend(); it++ )
        {
            appednState = redisGetReply(rconnection->rconnect,
                                        reinterpret_cast<void**>(&reply)
                                       );

            if(appednState == REDIS_ERR) // REDIS_ERR == -1
                std::cout << "Error: " << it->first.first.data() << std::endl;
        
            freeReplyObject(reply);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << " : " << errno << '\n';
        freeReplyObject(reply);
    }
}

void RedisCommand::Write( RedisConnection* rconnection, 
                          std::array<char ,512> arr
                        )
{
    try
    {
        reply = static_cast<redisReply*>(redisCommand(rconnection->rconnect,
                                                      arr.data()
                                                     )
                                        );
        
        std::cout << reply->str << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

void RedisCommand::Read( RedisConnection* rconnection, 
                         std::string_view key
                        )
{
    try
    {
        reply = static_cast<redisReply*>(redisCommand(rconnection->rconnect,
                                                      "GET key0",
                                                      key.data()
                                                     )
                                        );
        std::cout << "ReadData: " << reply->str << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

void RedisCommand::Authentication( RedisConnection* rconnection )
{
    try
    {
        reply = static_cast<redisReply*>(redisCommand(rconnection->rconnect,
                                                      "AUTH BarbaDDoSoperat10n"
                                                      //key.data()
                                                     )
                                        );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void RedisCommand::GetAllData( RedisConnection* rconnection,
                               std::map< std::pair<std::string,std::string>,
                                         std::string
                                       >& key_value)
{

    key_value.clear();
    try
    {
        reply = static_cast<redisReply*>(redisCommand(rconnection->rconnect,
                                                      "KEYS *"));
        redisReply *temp;
        for( size_t i = 0; i < reply->elements; i++ )
        {
            temp = static_cast<redisReply*>(redisCommand(rconnection->rconnect,
                                                         "HGETALL %b", &reply->element[i]->str[0],//8)
                                                         (size_t) strlen(reply->element[i]->str))
                                            );

            for(size_t j = 0; j < temp->elements; j = j+2)
            {
                key_value.try_emplace(std::make_pair(std::string(reply->element[i]->str), 

                                                     std::string{temp->element[j]->str, 
                                                                sizeof(temp->element[j]->str) } ),

                                                     std::string{temp->element[j+1]->str, 
                                                                sizeof(temp->element[j+1]->str) }
                                     );
            }
            freeReplyObject(temp);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}