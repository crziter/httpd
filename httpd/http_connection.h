#ifndef __http_connection_h__
#define __http_connection_h__

#include <string>
#include "port.h"
#include "tcp_socket.h"
#include "http_request.h"
#include "http_response.h"

class http_connection
{
public:
    http_connection(tcp_socket *sock);

    void append(vector_char& data);

    /**
     * Check if this connection finished received a request
     */
    bool has_request_pending();
    http_request next_request();

private:
    std::string _data;
    tcp_socket *_socket;
};

#endif