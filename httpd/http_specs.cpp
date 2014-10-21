#include "http_specs.h"

const char * http_method_str[] = {
    "GET",
    "POST",
    "PUT"
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

const char * http_header_str(header_list header)
{
    switch (header)
    {
    case CONTENT_TYPE:
        return "Content-Type";

    case CONTENT_LENGTH:
        return "Content-Length";

    case HOST:
        return "Host";

    case SERVER:
        return "Server";
    }

    return "";
}

const char * content_type_str(content_type type)
{
    switch (type)
    {
    case TEXT_HTML:
        return "text/html";
    }

    return "";
}
