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

    host_info() {
        location = "";
        domain = "";
    }
};

struct cgi_info
{
    std::string ext;
    std::string cmd;
};

class configuration
{
public:
    configuration();
    ~configuration();

    bool load(std::string& conf_file);
    bool load_mime(std::string& mime_file);

    host_info* config_for_host(std::string& host);

    cgi_info * cgi_for_ext(std::string ext);
    
    ushort http_port();
    ushort https_port();

    std::string& mime_of(std::string ext);

    std::string& listen_address();
    int num_workers();

    std::string& cert_file();
    std::string& key_file();

    std::string& server_name();

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

    std::string _cert_file;
    std::string _key_file;

    std::deque<cgi_info> _cgis;
};

#endif