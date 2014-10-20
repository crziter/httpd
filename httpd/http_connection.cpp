#include <iostream>
#include <regex>
#include "http_connection.h"
#include "http_specs.h"

using std::cout;
using std::endl;

void http_connection::append(vector_char& data)
{
    if (data.size() > 0)
        _data.append(&data[0], data.size());
}

bool http_connection::has_request_pending()
{
    std::regex fn_reg("(?:\\r\\n\\r\\n|\\n\\n)", std::regex_constants::ECMAScript);
    bool found = std::regex_search(_data, fn_reg);

    return found;
}

http_request http_connection::next_request()
{
    std::string request_string;

    if (has_request_pending()) {
        std::regex fn_reg("[^]*?" TWO_NEWLINE);
        std::smatch m;
        if (std::regex_search(_data, m, fn_reg)) {
            std::string request_packet = m.str();
            std::regex fn_method("^(\\w+)");
            std::smatch m_method;
            if (std::regex_search(request_packet, m_method, fn_method)) {
                if (m_method.size() > 0) {
                    std::string method = m_method[0].str();
                    if (_stricmp(method.c_str(), http_method_str[http_method::GET]) == 0) {
                        // Get the real request data
                        request_string = request_packet.substr(0, request_packet.length() - 4);
                        _data = m.suffix().str();
                    }
                    else if (_stricmp(method.c_str(), http_method_str[http_method::POST]) == 0) {
                        // Check if full request (including content) received
                        std::regex fn_content("Content-Length\\s*?:\\s*?(\\d+)" TWO_NEWLINE);
                        std::smatch m_length;
                        if (std::regex_search(request_packet, m_length, fn_content)) {
                            std::string length = m_length[1].str();
                            int i_length = std::stoi(length);

                            // TODO: Check if length not digit then drop request
                            if (_data.length() - request_packet.length() >= i_length) {
                                request_string = _data.substr(0, request_packet.length() + i_length);
                                _data = _data.substr(request_packet.length() + i_length);
                            }
                        }
                    }
                }
            }
        }
    }

    http_request rq;
    rq.parse(request_string);
    return rq;
}

http_connection::http_connection(socket_interface *sock)
    :_response(*this)
{
    _socket = sock;
}

http_response& http_connection::get_response()
{
    return _response;
}

socket_interface& http_connection::socket()
{
    return *_socket;
}
