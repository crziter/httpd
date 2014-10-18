#ifndef __http_uri_h__
#define __http_uri_h__

#include <string>

class http_uri
{
public:
    void decode(std::string& data);
    std::string& uri();

private:
    std::string _uri;
};

#endif