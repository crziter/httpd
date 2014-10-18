#ifndef __http_server_h__
#define __http_server_h__

#include "tcp_server.h"
#include "dispatcher.h"

class http_server
{
public:
    http_server();
    ~http_server();

    int start(const char_ptr addr, ushort port);
    void stop();

private:
    tcp_server *_server;
    dispatcher *_dispatcher;
};

#endif