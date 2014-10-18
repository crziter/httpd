#ifndef __http_task_h__
#define __http_task_h__

#include <condition_variable>
#include <functional>
#include <mutex>
#include "http_request.h"
#include "http_response.h"


class http_task
{
public:
    http_task(http_request& rq, http_response& rp);
    void run();
//     void wait_for_finish();
//     std::condition_variable& cond_var();

private:
    http_request _request;
    http_response _response;

//     std::condition_variable _cond_var;
//     bool _finished = false;
};

typedef http_task * http_task_ptr;

#endif