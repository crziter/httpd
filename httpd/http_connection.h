#ifndef __http_connection_h__
#define __http_connection_h__

#include <string>
#include "port.h"
#include "socket_interface.h"
#include "http_request.h"
#include "http_response.h"

class http_connection
{
public:
    http_connection(socket_interface *sock);

    void append(vector_char& data);

    /**
     * Check if this connection finished received a request
     */
    bool has_request_pending();
    http_request next_request();
    http_response& get_response();

    socket_interface& socket();

private:
    std::string _data;
    socket_interface *_socket;
    http_response _response;
};

#endif