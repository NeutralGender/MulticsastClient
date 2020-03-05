#include "../../Include/AuthentificationProtocol/CryptoAssymetric/RSA.h"

RSA::RSA( const size_t& keysize ) : CryptoAssymetric(keysize)
{
}

RSA::~RSA()
{
}

void RSA::Key_generation()
{
    try
    {
        //std::cout << "KeySize: " << keysize << std::endl;
        params.GenerateRandomWithKeySize(rng, keysize);
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        // Init private key by pseudorandom: n, e, d;
        private_key.Initialize(
                                params.GetModulus(),
                                params.GetPublicExponent(),
                                params.GetPrivateExponent()
                             );

        //std::cout << "BitCount: " << private_key.GetModulus().BitCount() << std::endl;
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        // Init public key by pseudorandom: n, e;
        public_key.Initialize(
                                params.GetModulus(),
                                params.GetPublicExponent()
                             );

    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void RSA::LoadPublicKeyFromFile( const std::string& pubkey_filename )
{
   try
   {
        CryptoPP::ByteQueue queue;
        CryptoPP::FileSource file( pubkey_filename.c_str(), true );

        file.TransferTo( queue );
        queue.MessageEnd();

        public_key.Load(queue);
        //std::cout << "PublLoadFromServer:" << public_key.GetModulus() << std::endl;
   }
   catch(const std::exception& e)
   {
       std::cerr << e.what() << '\n';
   }
   

}

void RSA::LoadPrivateKeyFromFile( const std::string& private_filename )
{
   try
   {
        CryptoPP::ByteQueue queue;
        CryptoPP::FileSource file( private_filename.c_str(), true );

        file.TransferTo( queue );
        queue.MessageEnd();

        private_key.Load(queue);
   }
   catch(const std::exception& e)
   {
       std::cerr << e.what() << '\n';
   }
}

void RSA::SavingPublicKeyToFile(const std::string& pubkey_string)
{
    try
    {
       CryptoPP::FileSink out( pubkey_string.c_str() );
       public_key.DEREncode(out);
       out.MessageEnd();
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

void RSA::SavingPrivateKeyToFile(const std::string& private_string)
{
    try
    {
       CryptoPP::FileSink out( private_string.c_str() );
       private_key.DEREncode(out);
       out.MessageEnd();
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

void RSA::LoadPublicKeyFromString( const std::string& public_key_string )
{
    try
    {
        CryptoPP::ByteQueue queue;
        CryptoPP::StringSource ss(public_key_string, true, new CryptoPP::Base64Decoder);

        ss.TransferTo( queue );
        queue.MessageEnd();

        public_key.Load(queue);


    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void RSA::LoadPrivateKeyFromString( const std::string& private_key_string )
{
    try
    {
        CryptoPP::ByteQueue queue;
        CryptoPP::StringSource ss(private_key_string, true, new CryptoPP::Base64Decoder);

        ss.TransferTo( queue );
        queue.MessageEnd();

        private_key.Load(queue);

    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

void RSA::SavingPublicKeyToString( std::string& public_key_string )
{
    try
    {
        CryptoPP::Base64Encoder pkencode(new CryptoPP::StringSink(public_key_string));
        public_key.DEREncode(pkencode);
        pkencode.MessageEnd(); // needs to write up to the end
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void RSA::SavingPrivateKeyToString( std::string& private_key_string )
{
    try
    {
        CryptoPP::Base64Encoder priv_encode(new CryptoPP::StringSink(private_key_string));
        private_key.DEREncode(priv_encode);
        priv_encode.MessageEnd(); // needs to write up to the end
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

void RSA::Encrypt( const std::string& plaintext, std::string& ciphertext )
{
    try
    {
/*
        CryptoPP::RSAES_OAEP_SHA_Encryptor e( public_key );
        CryptoPP::StringSource ss1( plaintext,
                                    true,
                                    new CryptoPP::PK_EncryptorFilter
                                        ( 
                                            rng, // random generator
                                            e, // open exponent
                                            new CryptoPP::StringSink(ciphertext)
                                        )
                                  );
*/


        CryptoPP::RSAES_OAEP_SHA_Encryptor e( public_key );
        CryptoPP::StringSource ss1( plaintext,
                                    true,
                                    new CryptoPP::PK_EncryptorFilter
                                        ( 
                                            rng, // random generator
                                            e, // open exponent
                                            new CryptoPP::HexEncoder
                                                (
                                                    new CryptoPP::StringSink(ciphertext)
                                                )
                                        )
                                  );

        //std::cout << "BitCount: " << private_key.GetModulus().BitCount() << std::endl;

        //std::cout << "Ciphertext: " << std::endl;
        //std::cout << ciphertext << std::endl;
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

void RSA::Encrypt( const std::vector<byte>& plaintext, std::string& ciphertext )
{
    try
    {
/*
        CryptoPP::RSAES_OAEP_SHA_Encryptor e( public_key );
        CryptoPP::StringSource ss1( plaintext.data(),
                                    plaintext.size(),
                                    true,
                                    new CryptoPP::PK_EncryptorFilter
                                        ( 
                                            rng, // random generator
                                            e, // open exponent
                                            new CryptoPP::StringSink(ciphertext)
                                        )
                                  );
*/


        CryptoPP::RSAES_OAEP_SHA_Encryptor e( public_key );
        CryptoPP::StringSource ss1( plaintext.data(),
                                    plaintext.size(),
                                    true,
                                    new CryptoPP::PK_EncryptorFilter
                                        ( 
                                            rng, // random generator
                                            e, // open exponent
                                            new CryptoPP::HexEncoder
                                                (
                                                    new CryptoPP::StringSink(ciphertext)
                                                )
                                        )
                                  );

        //std::cout << "BitCount: " << private_key.GetModulus().BitCount() << std::endl;
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

void RSA::Encrypt( const std::vector<byte>& plaintext, std::vector<byte>& ciphertext )
{
    try
    {
/*
        CryptoPP::RSAES_OAEP_SHA_Encryptor e( public_key );
        CryptoPP::StringSource ss1( plaintext.data(),
                                    plaintext.size(),
                                    true,
                                    new CryptoPP::PK_EncryptorFilter
                                        ( 
                                            rng, // random generator
                                            e, // open exponent
                                            new CryptoPP::ArraySink(ciphertext.data(),
                                                                    plaintext.size())
                                        )
                                  );
*/


        CryptoPP::RSAES_OAEP_SHA_Encryptor e( public_key );
        CryptoPP::StringSource ss1( plaintext.data(),
                                    plaintext.size(),
                                    true,
                                    new CryptoPP::PK_EncryptorFilter
                                        ( 
                                            rng, // random generator
                                            e, // open exponent
                                            new CryptoPP::HexEncoder
                                                (
                                                    new CryptoPP::ArraySink(ciphertext.data(),
                                                                            plaintext.size())
                                                )
                                        )
                                  );

    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << "Encrypt Error! " << e.what() << '\n';
    }

}

std::string RSA::Encrypt(const std::string& plaintext)
{
     try
    { 
        std::string ciphertext;

/*
        CryptoPP::RSAES_OAEP_SHA_Encryptor e( public_key );
        CryptoPP::StringSource ss1
                                 (  plaintext.data(),
                                    true,
                                    new CryptoPP::PK_EncryptorFilter
                                    ( 
                                        rng, // random generator
                                        e, // open exponent
                                        new CryptoPP::StringSink(ciphertext)
                                    )
                                );
*/


        CryptoPP::RSAES_OAEP_SHA_Encryptor e( public_key );
        CryptoPP::StringSource ss1
                                 (  plaintext.data(),
                                    true,
                                    new CryptoPP::PK_EncryptorFilter
                                    ( 
                                        rng, // random generator
                                        e, // open exponent
                                        new CryptoPP::HexEncoder
                                            (
                                                new CryptoPP::StringSink(ciphertext)
                                            )
                                    )
                                );

        return ( ciphertext );
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
        return ("");
    }
}

void RSA::Decrypt( const std::string& ciphertext, std::string& plaintext )
{
    // RSA Decryption CryptoSystem m = loge(c) ( mod n )
    // d*e = 1 mod( (p-1)(q-1) ) => d = e^(-1) mod( (p-1)(q-1) ); (e,fi(n)) = 1;

    try
    {
/*
        CryptoPP::RSAES_OAEP_SHA_Decryptor d( private_key );
        CryptoPP::StringSource ss2( ciphertext.data(),
                                    true,
                                    new CryptoPP::PK_DecryptorFilter
                                    ( 
                                        rng, // random generator
                                        d, // closed exponent
                                        new CryptoPP::StringSink(plaintext)
                                    )
                                  );
*/


        CryptoPP::RSAES_OAEP_SHA_Decryptor d( private_key );
        CryptoPP::StringSource ss2( ciphertext.data(),
                                    true,
                                    new CryptoPP::HexDecoder
                                        (
                                            new CryptoPP::PK_DecryptorFilter
                                            ( 
                                                rng, // random generator
                                                d, // closed exponent
                                                new CryptoPP::StringSink(plaintext)
                                                )
                                        )
                                  );


        //std::cout << "Plaintext: " << std::endl;
        //std::cout << plaintext << std::endl;

    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void RSA::Decrypt( const std::vector<byte>& ciphertext, std::vector<byte>& plaintext )
{
    // RSA Decryption CryptoSystem m = loge(c) ( mod n )
    // d*e = 1 mod( (p-1)(q-1) ) => d = e^(-1) mod( (p-1)(q-1) ); (e,fi(n)) = 1;
    try
    {
/*
        CryptoPP::RSAES_OAEP_SHA_Decryptor d( private_key );
        CryptoPP::StringSource ss2( ciphertext.data(),
                                    ciphertext.size(),
                                    true,
                                    new CryptoPP::PK_DecryptorFilter
                                    ( 
                                        rng, // random generator
                                        d, // closed exponent
                                        new CryptoPP::ArraySink(plaintext.data(),
                                                                ciphertext.size())
                                    )
                                  );
*/


        CryptoPP::RSAES_OAEP_SHA_Decryptor d( private_key );
        CryptoPP::StringSource ss2( ciphertext.data(),
                                    ciphertext.size(),
                                    true,
                                    new CryptoPP::HexDecoder
                                        (
                                            new CryptoPP::PK_DecryptorFilter
                                            ( 
                                                rng, // random generator
                                                d, // closed exponent
                                                new CryptoPP::ArraySink(plaintext.data(),
                                                                        ciphertext.size())
                                                )
                                        )
                                  );

    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

std::string RSA::Decrypt( const std::string& ciphertext )
{
    try
    {
        std::string plaintext;
/*
        CryptoPP::RSAES_OAEP_SHA_Decryptor d( private_key );
        CryptoPP::StringSource ss2( ciphertext,
                                    true,
                                    new CryptoPP::PK_DecryptorFilter
                                    ( 
                                        rng, // random generator
                                        d, // closed exponent
                                        new CryptoPP::StringSink(plaintext)
                                    )
                                  );
*/


        CryptoPP::RSAES_OAEP_SHA_Decryptor d( private_key );
        CryptoPP::StringSource ss2( ciphertext,
                                    true,
                                    new CryptoPP::HexDecoder
                                        (
                                            new CryptoPP::PK_DecryptorFilter
                                                ( 
                                                    rng, // random generator
                                                    d, // closed exponent
                                                    new CryptoPP::StringSink(plaintext)
                                                )
                                        )
                                  );

        //std::cout << "Decrypt: " << plaintext << std::endl;

        return ( plaintext );
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
        return ("Wrong CryptoMaterials\n");
    }
}

void RSA::Sign( const std::vector<byte>& message, std::vector< byte >& signature )
{
    CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Signer signer( private_key );

    size_t length = signer.MaxSignatureLength();
    signature.resize(length);

    try
    {
        size_t size = signer.SignMessageWithRecovery(
                                                        rng,
                                                        (const byte*)message.data(),
                                                        message.size(),
                                                        NULL,
                                                        0,
                                                        (byte*)signature.data()
                                                    );
        signature.resize(size);
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

std::string RSA::Sign ( const std::string& message )
{
    CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Signer signer( private_key );

    std::string signature;
    size_t length = signer.MaxSignatureLength();
    signature.resize(length);

    try
    {
        size_t size = signer.SignMessageWithRecovery(
                                                        rng,
                                                        (const byte*)message.c_str(),
                                                        message.size(),
                                                        NULL,
                                                        0,
                                                        (byte*)signature.data()
                                                    );
        signature.resize(size);
        return ( signature );
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
        return ("");
    }
}

std::vector<byte> RSA::Sign ( const std::vector<byte>& message )
{
        
    CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Signer signer( private_key );
    std::vector<byte> signature;

    try
    {
        size_t length = signer.MaxSignatureLength();
        signature.resize(length);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
    try
    {
        size_t size = signer.SignMessageWithRecovery
                                    (
                                        rng,
                                        message.data(),
                                        message.size(),
                                        NULL,
                                        0,
                                        signature.data()
                                    );
        signature.resize(size);

        //std::cout << "Signature: " 
        //          << signature.size() << ":" 
        //          << signature.data() << std::endl;

        return ( signature );
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
        return ( signature );
    }
    
}


void RSA::Sign( const std::string& message, std::string& signature )
{
    CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Signer signer( private_key );

    try
    {
        size_t length = signer.MaxSignatureLength();
        signature.resize(length);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    try
    {
        size_t size = signer.SignMessageWithRecovery
                                    (
                                        rng,
                                        (const byte*)message.c_str(),
                                        message.size(),
                                        NULL,
                                        0,
                                        (byte*)signature.data()
                                    );
        signature.resize(size);
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

std::vector< byte > RSA::Verify( const std::vector< byte >& signature)
{
    try
    {
        CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Verifier verifier( public_key );

        std::vector< byte > result;
        result.resize(signature.size());

        CryptoPP::DecodingResult r = verifier.RecoverMessage
                                                    (
                                                        result.data(),
                                                        NULL,
                                                        0,
                                                        (const byte*)signature.data(),
                                                        signature.size()
                                                    );

        //std::cout << "Result: " << result.size() << ":" << result.data() << std::endl;
        //std::cout << "isValidCoding: " << r.isValidCoding << r.messageLength << std::endl;
        return ( result );

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return {};
    }

}

std::vector<byte> RSA::Verify( const std::string& signature)
{
    try
    {
        CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Verifier verifier( public_key );

        std::vector<byte> result;
        result.resize(signature.size());

        CryptoPP::DecodingResult r = verifier.RecoverMessage
                                                    (
                                                        (byte*)result.data(),
                                                        NULL,
                                                        0,
                                                        (const byte*)signature.data(),
                                                        signature.size()
                                                    );
        result.resize(r.messageLength);

        //std::cout << "Result: " << result.size() << ":";
        //std::cout << result.data() << std::endl;
        //std::cout << "isValidCoding: " << r.isValidCoding << ":" << r.messageLength << std::endl;
        return ( result );

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return {};
    }

}
