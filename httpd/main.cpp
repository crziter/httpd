#include <iostream>
#include "port.h"
#include "http_server.h"
#include "http_connection.h"
#include "http_processor.h"

const int NUM_WORKERS = 4;

class handler : public http_handler
{
public:
    handler()
        : _processor(http_processor(NUM_WORKERS))
    {}

    void on_request(http_connection& conn) override
    {
        http_request rq = conn.next_request();

//         IF_DEBUG({
//             std::cout << rq.method_str() << " to " << rq.uri().uri() << std::endl;
//         });

        _processor.process(rq, conn.get_response());
    }

private:
    http_processor _processor;
};

int main(int argc, char *argv[]) {
    
    std::cout << "Waiting for clients ..." << std::endl;

    {
        http_server http(new handler());
        http.start("0.0.0.0", 8080);

        std::cin.get();

        http.stop();
    }

    std::cout << "Closed!" << std::endl;

    return 0;
}