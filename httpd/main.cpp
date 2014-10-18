#include <iostream>
#include "http_server.h"

int main(int argc, char *argv[]) {
    http_server http;
    http.start("0.0.0.0", 8080);

    std::cout << "It's work!" << std::endl;
    std::cin.get();
    http.stop();
    return 0;
}