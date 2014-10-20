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

private:
    http_request _request;
    http_response _response;
};

typedef http_task * http_task_ptr;

#endif