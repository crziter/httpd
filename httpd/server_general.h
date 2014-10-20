#ifndef __server_general_h__
#define __server_general_h__

#include <memory>
#include <deque>
#include <thread>
#include <map>

#include "port.h"
#include "socket_interface.h"
#include "server_handler.h"

class server_general
{
public:
    server_general(tcp_handler_ptr handler);
    virtual ~server_general();

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
    server_handler*                        _handler;
    std::thread                         _threads;
    socket_t                            _servers;
    std::map<socket_t, socket_interface *>    _socks;
    bool                                _running;

    void process_event();

    virtual socket_interface* accept(socket_t serverfd, socket_t& sock) = 0;
};

#endif