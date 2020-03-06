#include "../Include/MulticastServer.h"

MulticastServer::MulticastServer()
{

}

MulticastServer::~MulticastServer()
{

}

void MulticastServer::Authentification( SendClient* socketClient,
                                        AesModeCBC* AES,
                                        HashSHA3* sha
                                      )
{
// Load Bob Public Key from file( me must trust this key )
    RSA Bob(2048);
    Bob.LoadPublicKeyFromFile("server_pubkey.dat");
// End Load Bob Public Key from file( me must trust this key )
    
// Generate RSA key pair for new connection to server
    RSA Alice(3072);
    Alice.Key_generation();
// End Generate RSA key pair for new connection to server

// Send Alice Login Encrypted By Bob Public Key
    std::string Alice_login; // Alice_login ciphertext is encoded in Hex
    Bob.Encrypt("Alice", Alice_login);
    int n = 0;

    n = write(socketClient->socketfd, Alice_login.data(), Alice_login.size() );
// End Send Alice Login Encrypted By Bob Key

// Send Alice public_key To Bob
    std::string Alice_public_key; // Key is encoded in Base64
    Alice.SavingPublicKeyToString(Alice_public_key);

    sleep(1);

    n = write( socketClient->socketfd, Alice_public_key.data(), Alice_public_key.size() );
// END Send Alice public_key To Bob

// Receive Encrypted by Alice public and Signed by Bob private Key from Bob
    sleep(1);

    std::string recv_message;
    recv_message.resize(768);

    n = read ( socketClient->socketfd, &recv_message[0], 768 );
    //std::cout << "Recv_message AES_KEY: " << recv_message.data() << std::endl;
    std::vector<byte> AES_KEY(Bob.Verify( Alice.Decrypt(recv_message) ) );
// End Receive Encrypted by Alice public and Signed by Bob private Key from Bob

// Receive Encrypted Signed IV by private key from Bob
    sleep(1);

    n = read ( socketClient->socketfd, &recv_message[0], 768 );
    std::vector<byte>AES_IV(Bob.Verify( Alice.Decrypt(recv_message) ) );
// End Receive Encrypted Signed IV by private key from Bob

// Receive Encrypted Signed Ticket by private key from Bob
    sleep(1);

    n = read ( socketClient->socketfd, &recv_message[0], 768 );
    this->AES_Ticket = (Bob.Verify( Alice.Decrypt(recv_message) ) );
    //std::cout << "Ticket: " << this->AES_Ticket.data() << std::endl << this->AES_Ticket.size() << std::endl;
// End Receive Encrypted Signed Ticket by private key from Bob

// Calculate Digest: SHA3_256( AES_Ticket + password );
    std::string digest;

    sha->AddDataToSHA3object(std::string{AES_Ticket.begin(), AES_Ticket.end()}+"I am Client");
    sha->SetDigestStringSHAsize(digest);
    sha->CalculateDigest(digest);
// End Calculate Digest: SHA3_256( AES_Ticket + password );

// Encrypt Hash3_256 by Ephemere Key Received from Bob and Send it to him
    std::string aes_cipher;
    //AesModeCBC AES(CryptoPP::AES::DEFAULT_KEYLENGTH, CryptoPP::AES::BLOCKSIZE);
    AES->Encrypt( AES_KEY, AES_IV, digest, aes_cipher );

    sleep(1);

    n = write( socketClient->socketfd, aes_cipher.data(), aes_cipher.size() );
// End Encrypt Hash3_256 by Ephemere Key Received from Bob

// Receive AES Permanent Key form Bob
    sleep(1);

    recv_message.resize(32);
    read( socketClient->socketfd, &recv_message[0], 32 );

    AES->Decrypt( AES_KEY, AES_IV, recv_message, this->aes_key );

    std::cout << "AES_KEY: ";
    std::cout << this->aes_key.data() << std::endl;
// END Receive AES Permanent Key form Bob

// Receive AES Permanent IV from Bob
    recv_message.resize(32);
    read( socketClient->socketfd, &recv_message[0], 32 );

    AES->Decrypt( AES_KEY, AES_IV, recv_message, this->AES_IV );

    std::cout << "AES_IV: ";
    std::cout << this->AES_IV.data();
    std::cout << std::endl;
// End Receive AES Permanent IV from Bob

}

void MulticastServer::ReceiveMsg( ReceiveSocketMulticast* receive,
                                  AesModeCBC* AES
                                )
{

    std::map<std::pair<std::string,std::string>,std::string> src;

    std::string recv; // string for all received packages
    std::string encod; // string for decrypted packages, which cat to plain
    std::string plain; // result string with all packages

    int count_receive = 0;
    socklen_t len = sizeof(receive->local);
    try
    {
        recv.resize( 65'535 );

        // Receive
        while( ( count_receive = recvfrom( receive->sockfd,
                                           &recv[0],
                                           65'535,
                                           0,
                                           (struct  sockaddr*)&receive->local,
                                           &len
                                        ) 
                ) > 1 
             )
        {
            
            recv.resize( count_receive );

        // Decrypt
            AES->Decrypt( this->aes_key, this->AES_IV,
                          recv, encod
                        );

            plain = std::move( encod );

        // Deserialization
            std::stringstream st(plain);
            boost::archive::text_iarchive ia(st);
            ia >> src;

        // Mix Data
            key_value.insert( src.begin(), src.end() );

            recv.resize( 65'535 );
            src.clear();
            plain.clear();
        }

        std::cout << key_value.size() << std::endl;
        std::cout << "Exit!" << std::endl;

/*
        //size_t count_receive = 0;
        while(
                len > 00 &&
                (count_receive = read(  
                                        receive->sockfd,
                                        (void*)recv.data(),
                                        512
                                    ) > 0 )
                                    
            )
        {
            len -= 512;
            encod += recv.data();
            std::cout << recv.data() << std::endl;
        }

        std::cout << "Encod: " << encod.size() << std::endl;

        
        AES->Decrypt( this->aes_key, this->AES_IV,
                      encod,
                      plain
                    );

        std::cout << plain << std::endl;
        std::cin.get();
        std::stringstream st(plain);
        
        boost::archive::text_iarchive ia(st);
        ia >> key_value;
*/  
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << " :" << errno << '\n';
        std::cout << "Plain: " << plain.size() << std::endl;
        std::cout << "Count_receive : " << count_receive << std::endl;
        //std::cout << "Sizeof : " << sizeof(encod.data()) << std::endl;
    }
    
}

void MulticastServer::SendMsg(SendSocketMulticast* send)
{
    std::stringstream ss;
    boost::archive::text_oarchive oa(ss);
    oa << key_value;    

    std::map<std::pair<std::string,std::string>,std::string> new_map;
    boost::archive::text_iarchive ia(ss);
    ia >> new_map;

    std::cout << "NewMap:" << new_map.size() << std::endl;
    for(auto it = new_map.cbegin(); it != new_map.cend(); it++ )
    {
        std::cout << std::dec << it->first.first.c_str() << ": " << it->second << std::endl;
    }

    std::cout << "Check->" << (key_value == new_map) << std::endl;
            

    try
    {
        int valid = 0;

        for(auto it = key_value.cbegin(); it != key_value.cend(); it++ )
        {
            if( ( valid = sendto( send->sockfd,
                      ss.str().data(),
                      sizeof(ss.str().data()),
                      0,
                      (struct sockaddr*)&send->group,
                      sizeof(send->group) 
                    ) 
            ) < 0 )
            throw std::runtime_error("Cannot Write Socket");
        }

        //std::cout << "Valid Send: " << valid << std::endl;

        arr.fill(0);
    }
    catch(const std::runtime_error& r)
    {
        std::cerr << r.what() << ": " << errno << '\n';
    }
}

void MulticastServer::WriteToDB(DBDriver *db_driver)
{
    db_driver->Write(arr);
}

void MulticastServer::ConnectToDb(DBDriver *db_driver)
{
    db_driver->ConnectToDb();
}

void MulticastServer::Authentication(DBDriver *db_driver)
{
    db_driver->Authentication();
}

void MulticastServer::ReadFromDB(DBDriver *db_driver)
{
    db_driver->Read();
}

void MulticastServer::GetAllData(DBDriver *db_driver)
{
    db_driver->GetAllData(key_value);
}

void MulticastServer::PipelineWrite(DBDriver *db_driver)
{
    db_driver->PipelineWrite(key_value);
}