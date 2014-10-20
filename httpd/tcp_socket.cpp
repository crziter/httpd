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

// std::string& tcp_socket::remote_ip()
// {
//     throw std::logic_error("The method or operation is not implemented.");
// }
// 
// ushort& tcp_socket::remote_port()
// {
//     throw std::logic_error("The method or operation is not implemented.");
// }

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






// 
// const std::string& tcp_socket::remote_ip()
// {
//     return _remote_ip;
// }
// 
// const ushort tcp_socket::remote_port()
// {
//     return _remote_port;
// }
// 
// int tcp_socket::send(const char_ptr buffer, int len, int pos /*= 0*/)
// {
//     return ::send(_sock, buffer + pos, len, 0);
// }
// 
// int tcp_socket::send(const std::string& msg)
// {
//     return this->send((const char_ptr)msg.c_str(), msg.length(), 0);
// }
// 
// int tcp_socket::send(char_ptr buff, int lenth)
// {
//     throw std::logic_error("The method or operation is not implemented.");
// }
// 
// 
// int tcp_socket::recv(char *buffer, int len, int pos /*= 0*/)
// {
//     return ::recv(_sock, buffer + pos, len, 0);
// }
// 
// int tcp_socket::recv(std::string& msg)
// {
//     msg.clear();
//     char buffer[1024 * 3];
//     int len = this->recv(buffer, sizeof(buffer), 0);
// 
//     if (len > 0)
//         msg.append(buffer, len);
// 
//     return len;
// }
// 
// int tcp_socket::recv(vector_char& buffer)
// {
//     buffer.clear();
// 
//     char buff[1024 * 3];
//     int len = this->recv(buff, sizeof(buff), 0);
// 
//     if (len > 0) {
//         buffer.resize(sizeof(buff));
//         buffer.assign(buff, buff + len);
//     }
// 
//     return len;
// }
// 
// int tcp_socket::recv(char_ptr buff, int length)
// {
//     throw std::logic_error("The method or operation is not implemented.");
// }
