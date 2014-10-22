#ifndef __http_uri_h__
#define __http_uri_h__

#include <string>

class http_uri
{
public:
    void parse(std::string& data);
    std::string& path();
    std::string& query();
    std::string file_extension();

private:
    std::string _resource;
    std::string _path;
    std::string _query;
    std::string _hash;

    std::string decode(std::string& msg);
};

#endif