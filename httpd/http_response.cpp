#include "http_response.h"

http_response::http_response(http_connection& conn)
    :_conn(conn)
{

}

void http_response::method(http_method method)
{
    _method = method;
}

void http_response::content(std::string& content)
{
    _content = content;
}

void http_response::header(http_header& headers)
{
    _header = headers;
}

unsigned int http_response::flush()
{
    return 0;
}
