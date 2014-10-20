#ifndef __socket_h__
#define __socket_h__

#include <vector>
#include <string>
#include "port.h"
#include "socket_interface.h"

class tcp_socket : public socket_interface
{
public:
    tcp_socket(socket_t sock, const char_ptr ip, ushort port);
    virtual ~tcp_socket();
    /*
    virtual int send(const char_ptr buffer, int len, int pos = 0);
    virtual int send(const std::string& msg);
    */
    int send(char_ptr buff, int lenth) override;

    /* virtual int recv(char *buffer, int len, int pos = 0);
    virtual int recv(std::string& msg);
    virtual int recv(vector_char& buffer); */

    int recv(char_ptr buff, int length) override;

//     const std::string& remote_ip();
//     const ushort remote_port();

    void close() override;

    std::string& remote_ip() override;

    ushort& remote_port() override;

protected:
    std::string _remote_ip;
    ushort _remote_port;
    socket_t _sock;
};

#endif