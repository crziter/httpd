#include <iostream>
#include "port.h"
#include "http_server.h"
#include "http_connection.h"
#include "http_processor.h"
#include "configuration.h"

class handler : public http_handler
{
public:
    handler(configuration& conf)
        : _processor(http_processor(conf.num_workers()))
    {}

    void on_request(http_connection& conn) override
    {
        http_request rq = conn.next_request();
        _processor.process(rq, conn.get_response());
    }

private:
    http_processor _processor;
};

int main(int argc, char *argv[]) 
{    
    std::cout << "Waiting for clients ..." << std::endl;
    configuration conf;
    conf.load(std::string("c:\\httpd\\httpd.conf"));

    {
        http_server http(new handler(conf));
        http.start(conf);

        std::cin.get();

        http.stop();
    }

    std::cout << "Closed!" << std::endl;

    return 0;
}