#include <sstream>
#include "http_response.h"

http_response::http_response(http_connection& conn)
    :_conn(conn)
{
    _status = http_status::OK;
    _sent = 0;
    _cgi = false;

    IF_DEBUG({
        _content = "Hello world!";
    });
}

void http_response::content(std::string& content)
{
    _content = content;
}

void http_response::header(http_header& headers)
{
    _header = headers;
}

http_header& http_response::header()
{
    return _header;
}

unsigned int http_response::flush()
{
    auto rs = _conn.socket().send((char_ptr)_data.c_str(), (int)_data.length() - _sent);

    if (rs > 0)
        _sent += rs;

    return rs;
}

void http_response::version(std::string& version)
{
    _version = version;
}

void http_response::status(http_status status)
{
    _status = status;
}

void http_response::build()
{
    std::stringstream stream;

    stream << _version << " " << _status << " " << http_status_str(_status) << "\r\n";

    stream << _header.build();
    
    int length;
    if (_cgi) {
        int cl = _content.length();
        auto cstr = _content.c_str();
        length = 0;

        for (int i = 0; i < cl; ++i) {
            if (cstr[i] == '\r' && cstr[i + 1] == '\n' && cstr[i + 2] == '\r' && cstr[i + 3] == '\n') {
                length = _content.length() - i - 4;
                break;
            }
        }
    }
    else {
        length = _content.length();
    }

    stream << "Content-Length: " << length << "\r\n";

    if (!_cgi) {
        stream << "\r\n";
    }

    stream << _content;

    _data = stream.str();
}

bool http_response::is_sent_all()
{
    return (_sent == _data.length());
}

void http_response::with_cgi()
{
    _cgi = true;
}
