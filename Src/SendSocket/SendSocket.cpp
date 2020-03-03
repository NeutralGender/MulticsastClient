#include "../../Include/SendSocket/SendSocket.h"

SendSocket::SendSocket( const std::string& s_addr_,
                        const short port_)
                      : s_addr( s_addr_ ),
                        port( port_ )
{
}

SendSocket::~SendSocket()
{
}
