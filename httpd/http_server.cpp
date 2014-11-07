#include <iostream>
#include <thread>
#include <map>
#include "http_server.h"
#include "http_connection.h"
#include "http_task.h"
#include "http_handler.h"

class lowlevel_handler : public server_handler
{
public:
    lowlevel_handler(http_handler *handler) {
        _handler = handler;
    }

    void on_accept(socket_interface & sock) override
    {
        IF_DEBUG({
            std::cout << "Client has connected" << std::endl;
        });

        _conns[&sock] = new http_connection(&sock);
    }

    void on_receive(socket_interface& sock, vector_char& data) override
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

    void on_close(socket_interface & sock) override
    {
        IF_DEBUG({
            std::cout << "Client is about to disconnect" << std::endl;
        });

        _conns.erase(&sock);
    }

private:
    typedef std::map<socket_interface *, http_connection *> map_socket_connection;
    map_socket_connection _conns;
    http_handler *_handler;
};

http_server::http_server(http_handler *handler, configuration& conf)
    :_conf(conf)
{
    IF_WINDOWS({
        WSADATA wsdata;
        WSAStartup(MAKEWORD(2, 2), &wsdata);
    });

    _http_handler = handler;

    _lowlevel_handler = new lowlevel_handler(handler);
    _tcp_server = new tcp_server(_lowlevel_handler);
    _ssl_server = new ssl_server(_lowlevel_handler);

    if (_ssl_server->init(_conf.cert_file(), _conf.key_file()) == false) {
        std::cout << "_ssl_server->init() error" << std::endl;
        delete _ssl_server;
        _ssl_server = nullptr;
    }
}

http_server::~http_server()
{
    if (_tcp_server != nullptr) {
        delete _tcp_server;
    }

    if (_ssl_server != nullptr) {
        delete _ssl_server;
    }

    delete _http_handler;

    IF_WINDOWS({
        WSACleanup();
    });

    delete _lowlevel_handler;
}

bool http_server::start()
{
    _tcp_server->start((char_ptr)_conf.listen_address().c_str(), _conf.http_port());
    _ssl_server->start((char_ptr)_conf.listen_address().c_str(), _conf.https_port());

    return true;
}

void http_server::stop()
{
    _tcp_server->stop_all();
    _ssl_server->stop_all();
}
