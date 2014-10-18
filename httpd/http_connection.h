#ifndef __http_connection_h__
#define __http_connection_h__

#include <string>
#include "port.h"

class http_connection
{
public:
    /**
     * Check if this connection finished received a request
     */
    bool finished_request();
    void append(vector_char& data);
    const std::string& request_data();

private:
    std::string _data;
    std::string _request;
};

#endif