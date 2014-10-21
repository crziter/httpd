#include <iostream>
#include "port.h"
#include "http_processor.h"
#include "http_task.h"

http_processor::http_processor(configuration& conf)
    : _conf(conf)
{
    _dispatcher = new dispatcher(conf.num_workers());
}

http_processor::~http_processor()
{
    delete _dispatcher;
}

void http_processor::process(http_request& rq, http_response& response)
{
    http_task_ptr t = new http_task(rq, response, _conf);
    _dispatcher->submit_task(t);
}
