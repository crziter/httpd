#include <regex>
#include <iostream>
#include "http_request.h"
#include "port.h"

using std::regex;
using std::string;
using std::regex_search;
using std::smatch;

bool http_request::parse(const std::string& data)
{
    string buff = data;
    regex rex_method("^(\\w+)\\s+?(\\S+)\\s+?(\\S+)\\s*?" NEWLINE);
    smatch match_method;
    if (regex_search(buff, match_method, rex_method)) {
        if (match_method.size() > 0) {
            string method = match_method[1].str();
            string uri = match_method[2].str();
            string version = match_method[3].str();

            this->_uri.parse(uri);
            this->_http_version = version;

            if (_stricmp(method.c_str(), http_method_str[http_method::GET]) == 0) {
                this->_method = http_method::GET;
            }
            else if (_stricmp(method.c_str(), http_method_str[http_method::POST]) == 0) {
                this->_method = http_method::POST;
            }

            buff = match_method.suffix().str();
        }
    }
    else return false;

    regex rex_header("^([a-zA-Z\\d\\-]+)\\s*:\\s*(.+)(?:\\r\\n|\\n|$)");
    smatch match_header;
    int content_length = 0;
    while (regex_search(buff, match_header, rex_header)) {
        string key = match_header[1].str();
        string value = match_header[2].str();

        if (_stricmp(key.c_str(), "Content-Length") == 0) {
            content_length = std::stoi(value);
        }
        else if (_stricmp(key.c_str(), "Host") == 0) {
            if (value.find_first_of(':') == string::npos) { // Not found
                _host = value;
                _port = 80;
            } else {
                regex reg_host("^\\s*([\\da-zA-Z\\-\\.]+):(\\d+)\\s*$");
                smatch match_host;
                if (regex_search(value, match_host, reg_host)) {
                    _host = match_host[1].str();
                    _port = std::stoi(match_host[2].str());
                }
            }
        }

        _header.append(key, value);
        buff = match_header.suffix().str();
    }

    if (this->_method == http_method::POST) {
        // TODO: May this check wrong (may non-capture will be captured :v)
        regex fn_content("(?:\\r\\n|\\n)([^]+)$");
        smatch m_content;
        if (regex_match(buff, m_content, fn_content)) {
            this->_content = m_content[1].str();
        }
    }
    
    return true;
}

http_uri& http_request::uri()
{
    return _uri;
}

std::string& http_request::version()
{
    return _http_version;
}

http_method http_request::method()
{
    return _method;
}

std::string http_request::method_str()
{
    return std::string(http_method_str[this->method()]);
}

std::string& http_request::content()
{
    return _content;
}

http_header& http_request::header()
{
    return _header;
}

std::string& http_request::host()
{
    return _host;
}



