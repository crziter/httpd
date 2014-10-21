#ifndef __http_error_h__
#define __http_error_h__

#include <iostream>
#include "file_operations.h"
#include "http_specs.h"

class http_error
{
public:
    std::string error(http_status status);
};

#endif