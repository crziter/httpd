#include <iostream>
#include "ssl_server.h"
#include <openssl/err.h>

ssl_server::ssl_server(server_handler *handler)
    :server_general(handler)
{

}

socket_interface* ssl_server::accept(socket_t serverfd, socket_t& sock)
{
    sockaddr_in info;
    socklen_t len = sizeof(struct sockaddr);
    socket_interface *si = nullptr;
    SSL* ssl;

    socket_t client = ::accept(_servers, (struct sockaddr *) &info, &len);
    if (client == INVALID_SOCKET) {
        // error
        IF_DEBUG({
            std::cout << "Accept new ssl client error!" << std::endl;
        });
    }
    else
    {
        ssl = SSL_new(_ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) != -1) {
            sock = client;
            si = new ssl_socket(ssl, inet_ntoa(info.sin_addr), ntohs(info.sin_port));
        } else {
            const char *errString = ERR_error_string(ERR_get_error(), NULL);
            std::cout << errString << std::endl;
            closesocket(client);
        }
    }

    return si;
}

bool ssl_server::init_server_ctx()
{
    SSL_METHOD *method;

    OpenSSL_add_all_algorithms();		                /* load & register all cryptos, etc. */
    SSL_load_error_strings();			                /* load all error messages */
    method = (SSL_METHOD *) SSLv23_server_method();		/* create new server-method instance */
    _ctx = SSL_CTX_new(method);			                /* create new context from method */
    if (_ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
        // abort();

        return false;
    }

    SSL_CTX_set_options(_ctx,
        SSL_OP_SINGLE_DH_USE |
        SSL_OP_SINGLE_ECDH_USE |
        SSL_OP_NO_SSLv2);

    return true;
}

bool ssl_server::load_certs(std::string& cert_file, std::string& key_file)
{
    /* set the local certificate from CertFile */
    if (SSL_CTX_use_certificate_file(_ctx, cert_file.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return false;
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if (SSL_CTX_use_PrivateKey_file(_ctx, key_file.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return false;
    }
    /* verify private key */
    if (!SSL_CTX_check_private_key(_ctx))
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        return false;
    }

    return true;
}

bool ssl_server::init(std::string& cert_file, std::string& key_file)
{
    SSL_library_init();
    if (init_server_ctx()) {
        if (load_certs(cert_file, key_file)) {
            return true;
        }
    }

    return false;
}

ssl_server::~ssl_server() {
}
