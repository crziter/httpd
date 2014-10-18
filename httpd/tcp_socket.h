#ifndef __socket_h__
#define __socket_h__

#include <vector>
#include <string>
#include "port.h"

class tcp_socket
{
public:
    tcp_socket(socket_t sock, const char_ptr ip, ushort port);
    virtual ~tcp_socket();

    virtual int send(const char_ptr buffer, int len, int pos = 0);
    virtual int send(const std::string& msg);

    virtual int recv(char *buffer, int len, int pos = 0);
    virtual int recv(std::string& msg);
    virtual int recv(vector_char& buffer);

    const std::string& remote_ip();
    const ushort remote_port();

    void close();

protected:
    std::string _remote_ip;
    ushort _remote_port;
    socket_t _sock;
};

#endif