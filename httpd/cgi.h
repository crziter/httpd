#ifndef __cgi_h__
#define __cgi_h__

#include <iostream>
#include "http_request.h"
#include "configuration.h"

class cgi
{
public:
    cgi(host_info& hi, cgi_info& ci, http_request& rq, configuration& conf);
    bool process(std::string& content);

private:
    host_info& _hi;
    cgi_info& _ci;
    http_request& _req;
    configuration& _conf;

    const std::string SERVER_NAME       = "SERVER_NAME";
    const std::string GATEWAY_INTERFACE = "GATEWAY_INTERFACE";
    const std::string REQUEST_URI       = "REQUEST_URI";
    const std::string SERVER_PROTOCOL   = "SERVER_PROTOCOL";
    const std::string REDIRECT_STATUS   = "REDIRECT_STATUS";
    const std::string SCRIPT_FILENAME   = "SCRIPT_FILENAME";
    const std::string SCRIPT_NAME       = "SCRIPT_NAME";
    const std::string REQUEST_METHOD    = "REQUEST_METHOD";
    const std::string QUERY_STRING      = "QUERY_STRING";    
    const std::string SERVER_SOFTWARE   = "SERVER_SOFTWARE";  
    const std::string CONTENT_LENGTH_CGI    = "CONTENT_LENGTH";

    void set_env(const std::string& name, std::string& value);

};

#endif