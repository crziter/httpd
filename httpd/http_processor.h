#ifndef __http_processor_h__
#define __http_processor_h__

#include "dispatcher.h"

class http_processor
{
public:
    http_processor();
    ~http_processor();

    void process();

private:
    dispatcher *_dispatcher;
};

#endif