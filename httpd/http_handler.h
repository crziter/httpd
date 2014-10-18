#ifndef __http_handler_h__
#define __http_handler_h__

#include "http_request.h"

class http_handler
{
public:
    virtual bool on_request(http_request& request) = 0;
};

#endif