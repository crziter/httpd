#include <iostream>
#include <thread>
#include <map>
#include "http_server.h"
#include "http_connection.h"
#include "http_task.h"

class http_handler : public tcp_handler
{
public:
    http_handler(dispatcher * dispacher) {
        _dispatcher = dispacher;
    }

    void on_accept(tcp_socket & sock) override
    {
        _conns[&sock] = new http_connection();
    }

    void on_receive(tcp_socket& sock, vector_char& data) override
    {
        auto conn = _conns.at(&sock);

        conn->append(data);
        while (conn->finished_request()) {
            process_request(conn->request_data());
        }
    }

    void on_close(tcp_socket & sock) override
    {
        _conns.erase(&sock);
    }

private:
    typedef std::map<tcp_socket *, http_connection *> map_socket_connection;
    map_socket_connection _conns;
    dispatcher *_dispatcher;

    void process_request(const std::string& request_data) {
        http_request rq;
        rq.parse(request_data);

        std::cout << rq.method_str() << " to " << rq.uri().uri() << " with content (" << rq.content() << ")" << std::endl;
        return;
    }
};

http_server::http_server()
{
    _dispatcher = new dispatcher(4);
    _server = new tcp_server(new http_handler(_dispatcher));
}

http_server::~http_server()
{
    delete _dispatcher;

    if (_server != nullptr)
        delete _server;
}

int http_server::start(const char_ptr addr, ushort port)
{
    return _server->start(addr, port);
}

void http_server::stop()
{
    _server->stop_all();
}
