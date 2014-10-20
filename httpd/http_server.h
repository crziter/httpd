#ifndef __http_server_h__
#define __http_server_h__

#include "tcp_server.h"
#include "http_handler.h"
#include "configuration.h"

class http_server
{
public:
    http_server(http_handler *handler);
    ~http_server();

    int start(const char_ptr addr, ushort port);
    int start(configuration& conf);

    void stop();

private:
    tcp_server *    _server;
    http_handler *  _http_handler;
    configuration   _conf;
};

#endif