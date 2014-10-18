#include "port.h"
#include "dispatcher.h"

dispatcher::dispatcher(int num_workers)
{
    _num_workers    = num_workers;
    _running        = true;

    /* _lst_tasks      = new std::deque<http_task_ptr>[num_workers];
    _cond_vars      = new std::condition_variable[num_workers];
    _threads        = new std::thread[num_workers]; */

    _thread_datas = new thread_data[num_workers];
    for (int i = 0; i < num_workers; ++i) {
        _threads[i] = std::thread(&dispatcher::thread_func, this, i);
    }
}

dispatcher::~dispatcher()
{
    stop_workers();
}

void dispatcher::stop_workers()
{
    _running = false;
    for (int i = 0; i < _num_workers; ++i) {
        submit_task(nullptr, i);
    }

    for (int i = 0; i < _num_workers; ++i) {
        if (_threads[i].joinable())
            _threads[i].join();
    }

    delete[] _threads;
    delete[] _thread_datas;
//     delete[] _lst_tasks;
//     delete[] _cond_vars;
}

void dispatcher::submit_task(http_task_ptr task, int task_queue /* = -1 */)
{
    int choosen_idx = 0;
    if (task_queue == -1) {
        auto min_deque = _thread_datas[0]._lst_tasks.size();
        for (int i = 1; i<_num_workers; ++i) {
            if (min_deque > _thread_datas[i]._lst_tasks.size()) {
                min_deque = _thread_datas[i]._lst_tasks.size();
                choosen_idx = i;
            }
        }
    }
    else {
        choosen_idx = task_queue;
    }

    {
        std::unique_lock<std::mutex> lk(_thread_datas[choosen_idx]._task_mutex);
        _thread_datas[choosen_idx]._lst_tasks.push_back(task);
    }

    _thread_datas[choosen_idx]._cond_vars.notify_all();
}

void dispatcher::thread_func(int num)
{
    std::mutex _thread_mutex;

    IF_DEBUG({
        std::unique_lock<std::mutex> lk(_global_mutex);
        std::cout << "Worker " << std::this_thread::get_id() << " started, waiting for job .." << std::endl;
    });

    while (_running) {
        if (_thread_datas[num]._lst_tasks.size() == 0) {
            std::unique_lock<std::mutex> lk(_thread_mutex);
            _thread_datas[num]._cond_vars.wait(lk, [&]{return _thread_datas[num]._lst_tasks.size() > 0; });
        }
        
        IF_DEBUG({
            std::unique_lock<std::mutex> lk(_global_mutex);
            std::cout << "Worker " << std::this_thread::get_id() << " got job" << std::endl;
        });

        http_task_ptr task = nullptr;
        {
            std::unique_lock<std::mutex> lk(_thread_datas[num]._task_mutex);
            task = _thread_datas[num]._lst_tasks.front();
            _thread_datas[num]._lst_tasks.pop_front();
        }

        if (task != nullptr) {
            auto func = task->task();
            func();
            task->cond_var().notify_all();

            delete task;
        }
    }
}
