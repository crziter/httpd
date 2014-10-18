#include <iostream>
#include "port.h"
#include "http_server.h"
#include "http_connection.h"

class handler : public http_handler
{
public:
    void on_request(http_connection& conn) override
    {
        http_request rq = conn.next_request();

        IF_DEBUG({
            std::cout << rq.method_str() << " to " << rq.uri().uri() << std::endl;
        });
    }

};

int main(int argc, char *argv[]) {
    const int NUM_WORKERS = 4;
    std::cout << "Waiting for clients ..." << std::endl;

    {
        http_server http(NUM_WORKERS, new handler());
        http.start("0.0.0.0", 8080);

        std::cin.get();

        http.stop();
    }

    return 0;
}