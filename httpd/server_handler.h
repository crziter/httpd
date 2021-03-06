#ifndef __server_handler_h__
#define __server_handler_h__

#include <vector>
#include "port.h"
#include "socket_interface.h"

class server_handler
{
public:
    virtual void on_accept(socket_interface& sock) = 0;
    virtual void on_receive(socket_interface& sock, vector_char& data) = 0;
    virtual void on_close(socket_interface& sock) = 0;
};

typedef server_handler * tcp_handler_ptr;

#endif