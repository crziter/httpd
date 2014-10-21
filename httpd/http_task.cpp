#include "http_task.h"

http_task::http_task(http_request& rq, http_response& rp, configuration& conf)
    :_request(rq), _response(rp), _conf(conf)
{
}

void http_task::run()
{
    IF_DEBUG({
        std::cout << "Handle " << _request.method_str() << " request to " << _request.uri().uri() << std::endl;
    });

    _response.version(_request.version());

    host_info* host = _conf.config_for_host(_request.host());

    if (host != nullptr) {
        std::string found = "Found :)";
        _response.content(found);
    }
    else {
        std::string not_found = "Host not found :)";
        _response.content(not_found);
    }

    _response.build();
    while (!_response.is_sent_all()){
        _response.flush();
    }
}
