#include "tcp_socket.h"

tcp_socket::~tcp_socket()
{
    close();
}

tcp_socket::tcp_socket(socket_t sock, const char_ptr ip, ushort port)
{
    _sock = sock;
    _remote_ip = std::string(ip);
    _remote_port = port;
}

void tcp_socket::close()
{
    ::closesocket(_sock);

    _sock = INVALID_SOCKET;
    _remote_port = 0;
    _remote_ip = std::string();
}

int tcp_socket::send(char_ptr buff, int lenth)
{
    return ::send(_sock, buff, lenth, 0);
}

int tcp_socket::recv(char_ptr buff, int length)
{
    return ::recv(_sock, buff, length, 0);
}

std::string& tcp_socket::remote_ip()
{
    return _remote_ip;
}

ushort& tcp_socket::remote_port()
{
    return _remote_port;
}