#ifndef __http_response_h__
#define __http_response_h__

#include "http_specs.h"
#include "http_header.h"

class http_connection;
class http_response
{
public:
    http_response(http_connection& conn);

    void method(http_method method);
    void header(http_header& headers);
    void content(std::string& content);

    unsigned int flush();

private:
    http_connection& _conn;
    http_method _method;
    http_header _header;
    std::string _content;
};

#include "http_connection.h"
#endif