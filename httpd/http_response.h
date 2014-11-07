#ifndef __http_response_h__
#define __http_response_h__

#include "http_specs.h"
#include "http_header.h"
#include "http_request.h"

class http_connection;
class http_response
{
public:
    http_response(http_connection& conn);

    void version(std::string& version);
    void status(http_status status);
    void header(http_header& headers);
    void content(std::string& content);
    void with_cgi();

    http_header& header();

    void build();

    unsigned int flush();
    bool is_sent_all();

private:
    http_connection& _conn;
    http_header _header;
    std::string _content;
    std::string _version;
    http_status _status;
    int _sent;
    
    std::string _data;
    bool _cgi;
};

#include "http_connection.h"
#endif