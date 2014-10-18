#ifndef __tcp_handler_h__
#define __tcp_handler_h__

#include <vector>
#include "port.h"
#include "tcp_socket.h"

class tcp_handler
{
public:
    virtual void on_accept(tcp_socket& sock) = 0;
    virtual void on_receive(tcp_socket& sock, vector_char& data) = 0;
    virtual void on_close(tcp_socket& sock) = 0;
};

typedef tcp_handler * tcp_handler_ptr;

#endif