#ifndef __http_handler_h__
#define __http_handler_h__

#include "http_connection.h"

class http_handler
{
public:
    virtual void on_request(http_connection& conn) = 0;
};

#endif