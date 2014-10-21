#ifndef __http_error_h__
#define __http_error_h__

#include <iostream>
#include "http_specs.h"

class http_error
{
public:
    static std::string error(http_status status);
};

#endif