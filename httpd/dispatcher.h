#ifndef __dispatcher_h__
#define __dispatcher_h__

#include <map>
#include <deque>
#include <thread>
#include <condition_variable>
#include "http_task.h"

class dispatcher
{
public:
    dispatcher(int num_workers);
    ~dispatcher();

    void submit_task(http_task_ptr task, int task_que = -1);
    void stop_workers();

private:
    int _num_workers;
    bool _running;

    std::thread *               _threads;
    std::deque<http_task_ptr> * _lst_tasks;
    std::condition_variable *   _cond_vars;
    std::mutex                  _global_mutex;

    void thread_func(int num);
};

#endif