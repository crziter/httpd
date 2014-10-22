#ifndef __content_service_h__
#define __content_service_h__

#include "configuration.h"
#include "http_uri.h"
#include "http_request.h"
#include "http_response.h"
#include "http_specs.h"

class content_service
{
public:
    content_service(http_request& request, configuration& conf);
    void process(http_response& resp);
    std::string& content();

private:
    configuration& _conf;
    http_request& _request;
    std::string _content;
};

#endif