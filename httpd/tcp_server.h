#ifndef __tcp_server_h__
#define __tcp_server_h__

#include <memory>
#include <deque>
#include <thread>
#include <map>

#include "port.h"
#include "socket_interface.h"
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
    bool start(const char_ptr addr, ushort port);

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
    tcp_handler*                        _handler;
    std::thread                         _threads;
    socket_t                            _servers;
    std::map<socket_t, socket_interface *>    _socks;
    bool                                _running;

    void process_event();
};

#endif