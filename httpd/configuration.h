#ifndef __configuration_h__
#define __configuration_h__

#include <iostream>
#include <string>
#include <map>
#include "port.h"

struct host_info
{
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
    host_info& config_for_host(std::string& host);
    
    ushort http_port();
    ushort https_port();

    std::string& listen_address();
    int num_workers();

private:
    /* host <-> location */
    std::map<std::string, host_info *> _hosts;
    ushort _http_port;
    ushort _https_port;
    int _workers;
    std::string _address;
    std::string _server;
};

#endif