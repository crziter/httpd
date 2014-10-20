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
};

class configuration
{
public:
    configuration();
    ~configuration();

    bool load(std::string& conf_file);
    host_info& get_config_for_host(std::string& host);
    ushort get_listen_port();
    std::string& get_listen_address();

private:
    /* host <-> location */
    std::map<std::string, host_info *> _hosts;
    ushort _http_port;
    ushort _https_port;
};

#endif