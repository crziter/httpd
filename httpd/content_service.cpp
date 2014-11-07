#include <regex>
#include <iostream>
#include "content_service.h"
#include "file_operations.h"
#include "http_error.h"
#include "port.h"
#include "cgi.h"

void content_service::process(http_response& resp)
{
    std::string ext = _request.uri().file_extension(); 
    http_status status = OK;

    cgi_info *ci = _conf.cgi_for_ext(ext);

    // TODO: Check if file exist

    if (ci != nullptr) {
        cgi c(_hi, *ci, _request, _conf);
        c.process(_content);
        _is_cgi = true;
    } else {
        std::string full_path = _hi.location + file_operations::convert_path(_request.uri().path());

        if (!file_operations::get_content(full_path, _content)) {
            // status = http_status::NOT_FOUND;
            _content = http_error::error(http_status::NOT_FOUND);
        }
        else {
            resp.header().append(http_header_str(header_list::CONTENT_TYPE), _conf.mime_of(ext));
        }
    }

    resp.status(status);
}

content_service::content_service(http_request& request, configuration& conf, host_info& hi)
    : _conf(conf), _request(request), _hi(hi)
{
    _content.clear();
    _is_cgi = false;
}

std::string& content_service::content()
{
    return _content;
}

bool content_service::is_cgi()
{
    return _is_cgi;
}
