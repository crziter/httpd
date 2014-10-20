#ifndef __http_specs_h__
#define __http_specs_h__

enum http_method {
    GET,
    POST
};

enum http_status {
    OK = 200
};

extern const char * http_method_str[];

const char * http_status_str(http_status status);

#endif