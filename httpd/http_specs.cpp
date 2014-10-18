#include "http_specs.h"

const char * http_method_str[] = {
    "GET",
    "POST"
};

const char * http_statuses[] = {
    "OK"
};

const char * http_status_str(http_status status)
{
    switch (status)
    {
    case OK:
        return http_statuses[0];
        break;
    }

    return "";
}
