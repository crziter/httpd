#ifndef __socket_wrapper_h__
#define __socket_wrapper_h__

#include <iostream>
#include "socket_interface.h"
#include "port.h"

class socket_wrapper
{
public:
    socket_wrapper(socket_interface& sock);

//     std::string recv();
//     int recv(std::string& msg);
    int recv(vector_char& buffer);

private:
    socket_interface& _sock;
};
#endif