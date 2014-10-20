#include "http_task.h"

http_task::http_task(http_request& rq, http_response& rp)
    :_request(rq), _response(rp)
{
}

void http_task::run()
{
    IF_DEBUG({
        std::cout << "Handle " << _request.method_str() << " request to " << _request.uri().uri() << std::endl;
    });

    _response.version(_request.version());
    _response.build();

    while (!_response.is_sent_all()){
        _response.flush();
    }

}
