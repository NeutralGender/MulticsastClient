#ifndef _CRYPTOSYMMETRIC
#define _CRYPTOSYMMETRIC

#include "crypto++/secblock.h"

#include <vector>
#include <string>

typedef unsigned char byte;

class CryptoSymmetric
{
protected:
    const size_t& key_length;
public:
    CryptoSymmetric( const size_t& key_length_ );
    ~CryptoSymmetric();
    virtual void GenerateKey(std::vector<byte>& key) = 0;

    virtual void Encrypt( const std::vector<byte>& key,
                          const std::vector<byte>& iv,
                          const std::string& plaintext,
                          std::string& encoded) = 0;
                         
    virtual void Decrypt( const std::vector<byte>& key,
                          const std::vector<byte>& iv,
                          const std::string& encoded,
                          std::string& plaintext) = 0;

    friend class SendSocket;
};

#endif