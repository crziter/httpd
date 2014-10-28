#ifndef __http_server_h__
#define __http_server_h__

#include "ssl_server.h"
#include "tcp_server.h"
#include "http_handler.h"
#include "configuration.h"

class http_server
{
public:
    http_server(http_handler *handler, configuration& conf);
    ~http_server();

    /*bool start(const char_ptr addr, ushort port);*/
    bool start();

    void stop();

private:
    server_handler *_lowlevel_handler;
    tcp_server *    _tcp_server;
    ssl_server *    _ssl_server;

    http_handler *  _http_handler;
    configuration   _conf;
};

#endif