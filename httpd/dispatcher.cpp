#include "port.h"
#include "dispatcher.h"

dispatcher::dispatcher(int num_workers)
{
    _num_workers    = num_workers;
    _running        = true;

    _lst_tasks      = new std::deque<http_task_ptr>[num_workers];
    _cond_vars      = new std::condition_variable[num_workers];
    _threads        = new std::thread[num_workers];

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
    delete[] _lst_tasks;
    delete[] _cond_vars;
}

void dispatcher::submit_task(http_task_ptr task, int task_queue /* = -1 */)
{
    int choosen_idx = 0;
    if (task_queue == -1) {
        auto min_deque = _lst_tasks[0].size();
        for (int i = 1; i<_num_workers; ++i) {
            if (min_deque > _lst_tasks[i].size()) {
                min_deque = _lst_tasks[i].size();
                choosen_idx = i;
            }
        }

        _lst_tasks[choosen_idx].push_back(task);
    }
    else {
        choosen_idx = task_queue;
        _lst_tasks[task_queue].push_back(task);
    }

    _cond_vars[choosen_idx].notify_all();
}

void dispatcher::thread_func(int num)
{
    std::mutex _thread_mutex;

    IF_DEBUG({
        std::cout << "Worker " << std::this_thread::get_id() << " started, waiting for job .." << std::endl;
    });

    while (_running) {
        if (_lst_tasks[num].size() == 0) {
            std::unique_lock<std::mutex> lk(_thread_mutex);
            _cond_vars[num].wait(lk, [&]{return _lst_tasks[num].size() > 0; });
        }
        
        IF_DEBUG({
            std::unique_lock<std::mutex> lk(_global_mutex);
            std::cout << "Worker " << std::this_thread::get_id() << " got job" << std::endl;
        });

        http_task_ptr task = _lst_tasks[num].front();
        if (task != nullptr) {
            _lst_tasks[num].pop_front();
            auto func = task->task();
            func();
            task->cond_var().notify_all();

            delete task;
        }
    }
}
