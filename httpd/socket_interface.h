#ifndef __socket_interface_h__
#define __socket_interface_h__

#include <iostream>
#include "port.h"

class socket_interface
{
public:
    virtual ~socket_interface();

    /**
     * Send data to connection
     */
    virtual int send(char_ptr buff, int lenth) = 0;

    /**
     * Receive data from connection
     */
    virtual int recv(char_ptr buff, int length) = 0;

    /**
     * Get remote ip
     */
    virtual std::string& remote_ip() = 0;

    /**
     * Get remote port
     */
    virtual ushort& remote_port() = 0;

    /**
     * Close connection
     */
    virtual void close() = 0;
};

#endif