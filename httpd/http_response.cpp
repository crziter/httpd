#include <sstream>
#include "http_response.h"

http_response::http_response(http_connection& conn)
    :_conn(conn)
{
    _status = http_status::OK;
    _sent = 0;

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
    //header;
    stream << "Content-Length: " << _content.length() << "\r\n";
    stream << "\r\n" << _content;

    _data = stream.str();
}

bool http_response::is_sent_all()
{
    return (_sent == _data.length());
}
