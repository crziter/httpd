#ifndef __tcp_server_h__
#define __tcp_server_h__

#include <iostream>
#include "socket_interface.h"
#include "server_general.h"
#include "server_handler.h"

class tcp_server :public server_general
{
public:
    tcp_server(server_handler *handler);
    socket_interface* accept(socket_t serverfd, socket_t& sock) override;
};

#endif