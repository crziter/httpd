#include "tcp_server.h"
#include "tcp_socket.h"

socket_interface* tcp_server::accept(socket_t serverfd, socket_t& sock)
{
    sockaddr_in info;
    socklen_t len = sizeof(struct sockaddr);
    socket_t client = ::accept(_servers, (struct sockaddr *) &info, &len);
    socket_interface *si = nullptr;

    if (client == INVALID_SOCKET) {
        // error
        IF_DEBUG({
            std::cout << "Accept new client error!" << std::endl;
        });
    }
    else
    {
        sock = client;
        si = new tcp_socket(client, inet_ntoa(info.sin_addr), ntohs(info.sin_port));
    }

    return si;
}

tcp_server::tcp_server(server_handler *handler)
    : server_general(handler)
{

}
