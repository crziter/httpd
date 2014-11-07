#ifndef __tcp_socket_h__
#define __tcp_socket_h__

#include <vector>
#include <string>
#include "port.h"
#include "socket_interface.h"

class tcp_socket : public socket_interface
{
public:
    tcp_socket(socket_t sock, const char_ptr ip, ushort port);
    virtual ~tcp_socket();

    
    int send(char_ptr buff, int lenth) override;
    int recv(char_ptr buff, int length) override;

    void close() override;

    std::string& remote_ip() override;

    ushort& remote_port() override;

protected:
    std::string _remote_ip;
    ushort _remote_port;
    socket_t _sock;
};

#endif