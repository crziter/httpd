#ifndef __http_server_h__
#define __http_server_h__

#include "tcp_server.h"
#include "dispatcher.h"
#include "http_handler.h"

class http_server
{
public:
    http_server(int num_workers, http_handler *handler);
    ~http_server();

    int start(const char_ptr addr, ushort port);
    void stop();

private:
    tcp_server *    _server;
    dispatcher *    _dispatcher;
    http_handler *  _http_handler;
    int             _num_workers;
};

#endif