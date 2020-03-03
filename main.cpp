#include <iostream>

#include "Include/MulticastServer.h"
#include "Include/SocketMulticast/ReceiveSocketMulticast.h"
#include "Include/DBDriver/RedisDriver.h"

#include "Include/SendSocket/SendClient.h"

//Crypto Systems:
#include "Include/AuthentificationProtocol/CryptoHash/HashSHA3_256.h"
#include "Include/AuthentificationProtocol/CryptoSymmetric/AesModeCBC.h"

int main()
{
    AesModeCBC AES( 16, 16 );
    HashSHA3 sha3;

    // Authentification
    SendClient sc( "192.168.56.106", 8000 );
    sc.CreateSocket();
    sc.InitSocket();
    sc.Connect();

    MulticastServer m;
    m.Authentification( &sc, &AES, &sha3 );

    // DB object
    RedisDriver redis("127.0.0.1", 6379);

    // ReceiveSocket receive( "local", "group", port );
    /*
        local means interface by which you will receive/send multicast datagrams
            ( you init local interface like eth for multicast )
            ( see your interfaces in /etc/network/interfaces )
        group means multicast address, server send datagrams on this address
              and Clients receives using this group address
    */
    ReceiveSocketMulticast receiveMulticast("192.168.56.111", "225.1.1.1", 8000);
    receiveMulticast.Create_Socket();
    receiveMulticast.SetReuseAddr();
    receiveMulticast.Bind();
    receiveMulticast.JoinToGroup();
    
    m.ConnectToDb(&redis);
    //m.Authentication(&redis);
    while(true)
    {
        m.ReceiveMsg( &receiveMulticast, &AES );
        m.PipelineWrite(&redis); // Write to DB using Pipeline mode
    }

    return 0;
}