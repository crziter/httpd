#ifndef __configuration_h__
#define __configuration_h__

#include <iostream>
#include <string>
#include <deque>
#include <map>
#include "port.h"

struct host_info
{
    std::string domain;
    std::string location;
    std::string cert_file;
    bool ssl;

    host_info() {
        ssl = false;
        cert_file = "";
        location = "";
    }
};

class configuration
{
public:
    configuration();
    ~configuration();

    bool load(std::string& conf_file);
    bool load_mime(std::string& mime_file);

    host_info* config_for_host(std::string& host);
    
    ushort http_port();
    ushort https_port();

    std::string& mime_of(std::string ext);

    std::string& listen_address();
    int num_workers();

private:
    /* host <-> location */
    std::deque<host_info> _hosts;
    ushort _http_port;
    ushort _https_port;
    int _workers;
    std::string _address;
    std::string _server;

    std::map<std::string, std::string> _mime_types;
    std::string _plain_text;
};

#endif