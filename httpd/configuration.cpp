#include <iostream>
#include <fstream>
#include <regex>
#include "configuration.h"

using std::regex;
using std::regex_search;
using std::smatch;
using std::ifstream;
using std::ios;
using std::cout;

bool configuration::load(std::string& conf_file)
{
    ifstream f;
    char *buff = nullptr;
    f.open(conf_file);
    if (f.is_open()) {
        int length;
        f.seekg(0, ios::end);
        length = f.tellg();
        buff = new char[length + 1];
        f.seekg(0, ios::beg);

        f.read(buff, length);
        buff[length] = 0;
        f.close();
    }

    if (buff != nullptr) {
        std::string conf_string = std::string(buff);
        delete[] buff;

        regex reg_address("address\\s*=\\s*([\\d\\.]+)\\s*");
        smatch match_address;
        if (regex_search(conf_string, match_address, reg_address)) {
            _address = match_address[1].str();

            IF_DEBUG({
                cout << "Listen address: " << _address << std::endl;
            });
        }

        regex reg_http_port("http_port\\s*=\\s*(\\d+)\\s*");
        smatch match_http_port;
        if (regex_search(conf_string, match_http_port, reg_http_port)) {
            _http_port = std::stoi(match_http_port[1].str());

            IF_DEBUG({
                cout << "Http port: " << _http_port << std::endl;
            });
        }

        regex reg_https_port("https_port\\s*=\\s*(\\d+)\\s*");
        smatch match_https_port;
        if (regex_search(conf_string, match_https_port, reg_https_port)) {
            _https_port = std::stoi(match_https_port[1].str());

            IF_DEBUG({
                cout << "Https port: " << _https_port << std::endl;
            });
        }

        regex reg_workers("num_workers\\s*=\\s*(\\d+)\\s*");
        smatch match_workers;
        if (regex_search(conf_string, match_workers, reg_workers)) {
            _workers = std::stoi(match_workers[1].str());

            IF_DEBUG({
                cout << "Number of workers: " << _workers << std::endl;
            });
        }

        regex reg_server("server_name\\s*=\\s*([a-zA-Z\\-\\._]+)\\s*(?:\\r|\\n|\\s)");
        smatch match_server;
        if (regex_search(conf_string, match_server, reg_server)) {
            _server = match_server[1].str();

            IF_DEBUG({
                cout << "Server name: " << _server << std::endl;
            });
        }

        std::string conf_hosts = conf_string;
        regex reg_hosts("host\\s+\\\"(\\s*[a-zA-Z\\.\\-]+\\s*)\\\"\\s*\\{([^]+?)\\}");
        smatch match_hosts;
        while (regex_search(conf_hosts, match_hosts, reg_hosts)) {
            for (int i = 1; i < match_hosts.size(); i+=2) {
                std::string domain = match_hosts[i].str();
                std::string host_conf = match_hosts[i + 1].str();
                host_info *host = nullptr;
                
                IF_DEBUG({
                    cout << "Host: '" << domain << "'" << std::endl;
                });

                regex reg_loc("location\\s*=\\s*([^]+?)\\s*(?:\\r|\\n)");
                smatch match_loc;
                if (regex_search(host_conf, match_loc, reg_loc)) {
                    std::string location = match_loc[1].str();

                    // TODO: Check if location exists
                    host = new host_info();
                    host->location = location;

                    IF_DEBUG({
                        cout << "\tLocation: '" << location << "'" << std::endl;
                    });
                } else continue;

                regex reg_protocol("protocol\\s*=\\s*([a-z]+?)\\s*(?:\\r|\\n)");
                smatch match_protocol;
                if (regex_search(host_conf, match_protocol, reg_protocol)) {
                    std::string protocol = match_protocol[1].str();

                    IF_DEBUG({
                        cout << "\tProtocol: '" << protocol << "'" << std::endl;
                    });

                    if (protocol.compare("https") == 0) {
                        regex reg_cert("cert\\s*=\\s*([^]+?)\\s*(?:\\r|\\n)");
                        smatch match_cert;
                        std::string cert;

                        if (regex_search(host_conf, match_cert, reg_cert)) {
                            cert = match_cert[1].str();
                            host->ssl = true;
                            host->cert_file = cert;

                            IF_DEBUG({
                                cout << "\tCert: " << cert << std::endl;
                            });
                        }
                    }
                }

                if (host != nullptr)
                    _hosts[domain] = host;
            }

            conf_hosts = match_hosts.suffix().str();
        }

        return true;
    }

    return false;
}

configuration::configuration()
{
    _workers = 1;
    _http_port = 80;
    _https_port = 443;
    _address = "0.0.0.0";
}

configuration::~configuration()
{
    for (auto h : _hosts) {
        delete (host_info *)h.second;
    }

    _hosts.clear();
}

ushort configuration::http_port()
{
    return _http_port;
}

ushort configuration::https_port()
{
    return _https_port;
}

std::string& configuration::listen_address()
{
    return _address;
}

int configuration::num_workers()
{
    return _workers;
}
