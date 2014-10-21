#ifndef __http_processor_h__
#define __http_processor_h__

#include "dispatcher.h"
#include "http_connection.h"
#include "http_request.h"
#include "http_response.h"
#include "configuration.h"

class http_processor
{
public:
    http_processor(configuration& conf);
    ~http_processor();

    void process(http_request& rq, http_response& response);

private:
    dispatcher *_dispatcher;
    configuration& _conf;
};

#endif