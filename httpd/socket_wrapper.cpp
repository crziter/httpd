#include "socket_wrapper.h"

socket_wrapper::socket_wrapper(socket_interface& sock)
    : _sock(sock)
{

}

int socket_wrapper::recv(vector_char& buffer)
{
    buffer.clear();

    char buff[1024 * 3];
    int len = _sock.recv(buff, sizeof(buff));

    if (len > 0) {
        buffer.resize(sizeof(buff));
        buffer.assign(buff, buff + len);
    }

    return len;

}

