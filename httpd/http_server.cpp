#include <iostream>
#include <thread>
#include <map>
#include "http_server.h"
#include "http_connection.h"
#include "http_task.h"
#include "http_handler.h"

class lowlevel_handler : public tcp_handler
{
public:
    lowlevel_handler(http_handler *handler) {
        _handler = handler;
    }

    void on_accept(tcp_socket & sock) override
    {
        IF_DEBUG({
            std::cout << "Client has connected" << std::endl;
        });

        _conns[&sock] = new http_connection(&sock);
    }

    void on_receive(tcp_socket& sock, vector_char& data) override
    {
        auto conn = _conns.at(&sock);

        IF_DEBUG({
            std::cout << "Received " << data.size() << " bytes from client" << std::endl;
        });

        conn->append(data);
        while (conn->has_request_pending()) {
            _handler->on_request(*_conns[&sock]);
        }
    }

    void on_close(tcp_socket & sock) override
    {
        IF_DEBUG({
            std::cout << "Client is about to disconnect" << std::endl;
        });

        _conns.erase(&sock);
    }

private:
    typedef std::map<tcp_socket *, http_connection *> map_socket_connection;
    map_socket_connection _conns;
    http_handler *_handler;
};

http_server::http_server(http_handler *handler)
{
    IF_WINDOWS({
        WSADATA wsdata;
        WSAStartup(MAKEWORD(2, 2), &wsdata);
    });

    _http_handler = handler;
    _server = new tcp_server(new lowlevel_handler(handler));
}

http_server::~http_server()
{
    if (_server != nullptr) {
        delete _server;
        delete _http_handler;
    }

    IF_WINDOWS({
        WSACleanup();
    });
}

int http_server::start(const char_ptr addr, ushort port)
{
    return _server->start(addr, port);
}

int http_server::start(configuration& conf)
{
    _conf = conf;

    return _server->start((char_ptr)_conf.listen_address().c_str(), _conf.http_port());
}

void http_server::stop()
{
    _server->stop_all();
}
