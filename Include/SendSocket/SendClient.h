#ifndef _SEND_CLIENT
#define _SEND_CLIENT

#include "SendSocket.h"
#include "../../MulticastServer.h"

#include <string>

class SendClient : public SendSocket
{
public:
    SendClient( const std::string& s_addr_,
                const short port_);
    ~SendClient();

    virtual void CreateSocket() override;
    virtual void InitSocket() override;
    virtual void Connect() override;

    friend class MulticastServer;
};

#endif