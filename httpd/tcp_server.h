#ifndef __tcp_server_h__
#define __tcp_server_h__

#include <memory>
#include <deque>
#include <thread>
#include <map>

#include "port.h"
#include "tcp_handler.h"

class tcp_server
{
public:
    tcp_server(tcp_handler_ptr handler);
    virtual ~tcp_server();

    /**
     * Start a listen instance
     * @return -1 if error
     */
    int start(const char_ptr addr, ushort port);

    /**
     * Stop an server instance
     * @param instance_id ID which was returned from start()
     */
    void stop(int instance_id);

    /**
     * Stop the server
     */
    void stop_all();

protected:
    typedef std::thread * thread_ptr;
    typedef std::map<socket_t, tcp_socket *> map_sockets;

    std::unique_ptr<tcp_handler> _handler;
    std::deque<thread_ptr> _threads;
    std::vector<socket_t> _servers;
    map_sockets _socks;

    bool _running;

    void thread_func(socket_t server_fd);
};

#endif