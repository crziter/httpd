#include <regex>
#include <iostream>
#include "content_service.h"
#include "file_operations.h"
#include "http_error.h"
#include "port.h"

void content_service::process(http_response& resp)
{
    std::string ext = _request.uri().file_extension(); 
    http_status status = OK;

    if (_stricmp(ext.c_str(), "php") == 0) {
        // CALL CGI
    }
    else {
        host_info *hi = _conf.config_for_host(_request.host());

        std::string full_path = hi->location + file_operations::convert_path(_request.uri().path());

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

content_service::content_service(http_request& request, configuration& conf)
    : _conf(conf), _request(request)
{
    _content.clear();
}

std::string& content_service::content()
{
    return _content;
}
