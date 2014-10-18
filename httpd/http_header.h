#ifndef __http_header_h__
#define __http_header_h__

#include <string>
#include <map>

class http_header
{
public:
    void append(std::string& key, std::string& value);
    std::string& get(std::string& key);
    std::string build();

private:
    std::map<std::string, std::string> _headers;
};

#endif