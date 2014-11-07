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
        : _processor(http_processor(conf))
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
    configuration conf;
    std::string conf_file = std::string(CONFIG_FILE);
    if (conf.load(conf_file))
    {
        {
            std::string mime_f(MIME_FILE);
            conf.load_mime(mime_f);
        }
        
        // Loaded conf
        http_server http(new handler(conf), conf);
        if (http.start()) {
            std::cout << "Http server was started" << std::endl << "Waiting for clients ... " << std::endl;

            std::cin.get();
            http.stop();
        } else {
            std::cout << "Can not start http server" << std::endl;
        }
    } else {
        std::cout << "Couldn\'t load config file" << std::endl;
        return -1;
    }

    std::cout << "Closed!" << std::endl;
    std::cin.get();
    return 0;
}