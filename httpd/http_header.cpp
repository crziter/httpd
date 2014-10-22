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

    for (map<string, string>::iterator it = begin(_headers); it != end(_headers); ++it) {
        stream << it->first << ": " << it->second << "\r\n";
    }

    return stream.str();
}
