#include "http_task.h"

http_task::http_task(task_func task)
{
    _task = task;
}

void http_task::wait_for_finish()
{
    if (_finished) return;
    else {
        std::mutex _mutex;
        std::unique_lock<std::mutex> lk(_mutex);
        _cond_var.wait(lk, [&] {return _finished; });
    }
}

http_task::task_func& http_task::task()
{
    return _task;
}

std::condition_variable& http_task::cond_var()
{
    return _cond_var;
}
