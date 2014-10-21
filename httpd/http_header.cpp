#include <sstream>
#include "http_header.h"
using std::map;
using std::string;
using std::begin;
using std::end;

void http_header::append(std::string key, std::string value)
{
    _headers[key] = value;
}

// std::string& http_header::get(std::string& key)
// {
//     return _headers[key];
// }

std::string& http_header::get(std::string key)
{
    return _headers[key];
}

std::string http_header::build()
{
    std::stringstream stream;

    auto size = _headers.size();
    unsigned int index = 1;
    for (map<string, string>::iterator it = begin(_headers); it != end(_headers); ++it) {
        if (index < size)
            stream << it->first << ": " << it->second << "\r\n";
        else
            stream << it->first << ": " << it->second;
    }

    return stream.str();
}
