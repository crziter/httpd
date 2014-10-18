#ifndef __http_request_h__
#define __http_request_h__

#include <string>
#include "http_header.h"
#include "http_specs.h"
#include "http_uri.h"

class http_request
{
public:
    bool parse(const std::string& data);
    http_header& header();
    std::string& content();
    http_method method();
    std::string method_str();
    http_uri& uri();
    std::string& version();

private:
    http_method _method;
    http_uri _uri;
    std::string _http_version;

    http_header _header;
    std::string _content;
};

#endif