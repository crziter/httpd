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
    virtual int recv(char_ptr buff, int length) = 0;
    virtual std::string& remote_ip() = 0;
    virtual ushort& remote_port() = 0;

    virtual void close() = 0;
};

#endif