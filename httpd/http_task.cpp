#include "http_task.h"

http_task::http_task(http_request& rq, http_response& rp)
    :_rq(rq), _rp(rp)
{
}

// void http_task::wait_for_finish()
// {
//     if (_finished) return;
//     else {
//         std::mutex _mutex;
//         std::unique_lock<std::mutex> lk(_mutex);
//         _cond_var.wait(lk, [&] {return _finished; });
//     }
// }
// 
// std::condition_variable& http_task::cond_var()
// {
//     return _cond_var;
// }

void http_task::run()
{
    IF_DEBUG({
        std::cout << "Handle " << _rq.method_str() << " request to " << _rq.uri().uri() << std::endl;
    });
}
