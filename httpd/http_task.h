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
    typedef std::function<void()> task_func;

    http_task(task_func task);
    void wait_for_finish();
    task_func& task();
    std::condition_variable& cond_var();

private:
    task_func _task;
    std::condition_variable _cond_var;
    bool _finished = false;
};

typedef http_task * http_task_ptr;

#endif