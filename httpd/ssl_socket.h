#ifndef __ssl_socket_h__
#define __ssl_socket_h__

#include <iostream>
#include <openssl/ssl.h>
#include "socket_interface.h"

class ssl_socket : public socket_interface
{
public:
    ssl_socket(SSL* ssl, const char_ptr ip, ushort port);

    int send(char_ptr buff, int lenth) override;
    int recv(char_ptr buff, int length) override;
    std::string& remote_ip() override;
    ushort& remote_port() override;
    void close() override;

private:
    SSL *_ssl;
    std::string _ip;
    ushort _port;
};

#endif