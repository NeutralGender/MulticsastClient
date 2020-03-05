#ifndef _MulticastServer
#define _MulticastServer

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>
#include <map>
#include <array>
#include <vector>
#include <regex>

#include <boost/serialization/map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "DBDriver/DBDriver.h"

#include "AuthentificationProtocol/CryptoSymmetric/AesModeCBC.h"
#include "AuthentificationProtocol/CryptoAssymetric/RSA.h"
#include "AuthentificationProtocol/CryptoHash/HashSHA3_256.h"

#include "SendSocket/SendClient.h"
class SendClient;

#include "SocketMulticast/ReceiveSocketMulticast.h"
#include "SocketMulticast/SendSocketMulticast.h"

class MulticastServer
{
private:
    std::array<char,512> arr;
    std::map<std::pair<std::string,std::string>,std::string> key_value;
    std::vector< byte > aes_key;
    std::vector< byte > AES_IV;
    std::vector< byte > AES_Ticket;

public:
    MulticastServer();
    ~MulticastServer();

    void ReceiveMsg( ReceiveSocketMulticast* receive,
                     AesModeCBC* AES
                   );

    void SendMsg(SendSocketMulticast* send);
    void ConnectToDb(DBDriver *db_driver);
    void Authentication(DBDriver *db_driver);

    void Authentification( SendClient* socketClient,
                           AesModeCBC* AES,
                           HashSHA3* sha
                         );

    void WriteToDB(DBDriver *db_driver);
    void GetAllData(DBDriver *db_driver);
    void PipelineWrite(DBDriver *db_driver);
    void ReadFromDB(DBDriver *db_driver);
};




#endif