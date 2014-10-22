#ifndef __http_specs_h__
#define __http_specs_h__

enum http_method {
    GET,
    POST
};

enum http_status {
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404
};

enum header_list {
    CONTENT_TYPE,
    CONTENT_LENGTH,
    SERVER,
    HOST
};

enum content_type {
    TEXT_HTML
};

extern const char * http_method_str[];

const char * http_status_str(http_status status);
const char * http_header_str(header_list header);
const char * content_type_str(content_type type);

#endif