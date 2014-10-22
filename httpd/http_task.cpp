#include "http_task.h"
#include "http_error.h"
#include "content_service.h"

http_task::http_task(http_request& rq, http_response& rp, configuration& conf)
    :_request(rq), _response(rp), _conf(conf)
{
}

void http_task::run()
{
    IF_DEBUG({
        std::cout << _request.method_str() << " " << _request.uri().path() << "(" << _request.uri().query() << ")" << std::endl;
    });

    _response.version(_request.version());

    host_info* host = _conf.config_for_host(_request.host());

    if (host != nullptr) {
        content_service cs(_request, _conf);
        cs.process(_response);

//         _response.status(status);

        _response.content(cs.content());
    } else {
        _response.status(http_status::OK);
        std::string err_content = http_error::error(http_status::BAD_REQUEST);
        _response.content(err_content);
        _response.header().append(http_header_str(header_list::CONTENT_TYPE), content_type_str(content_type::TEXT_HTML));
    }

    _response.build();
    while (!_response.is_sent_all()){
        _response.flush();
    }
}
