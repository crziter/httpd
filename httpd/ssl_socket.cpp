#include "ssl_socket.h"

ssl_socket::ssl_socket(SSL* ssl, const char_ptr ip, ushort port)
{
    _ssl = ssl;
    _ip = std::string(ip);
    _port = port;
}

void ssl_socket::close()
{
    socket_t client = SSL_get_fd(_ssl);
    SSL_free(_ssl);
    closesocket(client);
}

int ssl_socket::send(char_ptr buff, int lenth)
{
    return SSL_write(_ssl, buff, lenth);
}

int ssl_socket::recv(char_ptr buff, int length)
{
    return SSL_read(_ssl, buff, length);
}

std::string& ssl_socket::remote_ip()
{
    return _ip;
}

ushort& ssl_socket::remote_port()
{
    return _port;
}

