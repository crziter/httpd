#ifndef __ssl_server_h__
#define __ssl_server_h__

#include <iostream>
#include <openssl/ssl.h>
#include "server_general.h"
#include "ssl_socket.h"

class ssl_server: public server_general
{
public:
    ssl_server(server_handler *handler);
    
    bool init(std::string& cert_file, std::string& key_file);
    socket_interface* accept(socket_t serverfd, socket_t& sock) override;

private:
    bool init_server_ctx();
    bool load_certs(std::string& cert_file, std::string& key_file);

    SSL_CTX *_ctx;
};

#endif