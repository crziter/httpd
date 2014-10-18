#include <iostream>
#include <vector>
#include "tcp_server.h"
#if defined(WIN32) || defined(_WIN32)
#pragma comment(lib, "ws2_32.lib")
#endif

tcp_server::tcp_server(tcp_handler_ptr handler)
{
    IF_WINDOWS({
        WSADATA wsdata;
        WSAStartup(MAKEWORD(2, 2), &wsdata);
    });

    _handler.reset(handler);
    _running = true;
}

tcp_server::~tcp_server()
{
    stop_all();

    for (map_sockets::iterator it = std::begin(_socks); it != std::end(_socks); ++it) {
        delete it->second;
    }

    IF_WINDOWS({
        WSACleanup();
    });
}

int tcp_server::start(const char_ptr addr, ushort port)
{
    sockaddr_in info;

    memset(&info, 0, sizeof(info));
    info.sin_addr.s_addr = inet_addr(addr);
    info.sin_family = AF_INET;
    info.sin_port = htons(port);

    socket_t sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        return -1;
    }

    if (::bind(sock, (struct sockaddr *) &info, sizeof(struct sockaddr)) == SOCKET_ERROR) {
        closesocket(sock);
        return -1;
    }

    if (::listen(sock, 10) == SOCKET_ERROR) {
        closesocket(sock);
        return -1;
    }

    _servers.push_back(sock);
    std::thread *th = new std::thread(&tcp_server::thread_func, this, sock);
    _threads.push_back(th);

    return _threads.size() - 1;
}

void tcp_server::thread_func(socket_t server_fd)
{
    unsigned int maxfds = 0;
    fd_set readset;

    timeval tv;
    tv.tv_usec = 500000;
    tv.tv_sec = 0;
    while (_running) {
        FD_ZERO(&readset);
        
        for (std::vector<socket_t>::iterator it = std::begin(_servers); it != std::end(_servers); ++it) {
            if (maxfds < *it) maxfds = *it;
            FD_SET(*it, &readset);
        }

        for (map_sockets::iterator it = std::begin(_socks); it != std::end(_socks); ++it) {
            if (maxfds < it->first) maxfds = it->first;
            FD_SET(it->first, &readset);
        }

        auto rs = ::select(maxfds + 1, &readset, nullptr, nullptr, &tv);
        if (rs < 0) {
            /* Error or exiting */

            IF_DEBUG({
                std::cout << "select error !!!" << std::endl;
            });
        }
        else if (rs == 0) {
            /* Time out */
        }
        else {
            if (FD_ISSET(server_fd, &readset)) {
                sockaddr_in info;
                socklen_t len = sizeof(struct sockaddr);

                socket_t client = ::accept(server_fd, (struct sockaddr *) &info, &len);

                if (client < 0) {
                    // error
                }
                else 
                {
                    maxfds = maxfds > client ? maxfds : client;

                    _socks[client] = new tcp_socket(client, inet_ntoa(info.sin_addr), ntohs(info.sin_port));
                    _handler->on_accept(*_socks.at(client));
                }

                FD_CLR(server_fd, &readset);
            }

            std::vector<socket_t> closed_list;
            for (map_sockets::iterator it = std::begin(_socks); it != std::end(_socks); ++it) {
                if (FD_ISSET(it->first, &readset)) {
                    
                    vector_char buffer;;
                    int len = it->second->recv(buffer);
                    
                    if (len <= 0) {
                        _handler->on_close(*(it->second));
                        closesocket(it->first);
                        closed_list.push_back(it->first);
                    }
                    else if (len > 0)
                        _handler->on_receive(*(it->second), buffer);
                }
            }

            for (std::vector<socket_t>::iterator it = std::begin(closed_list); it != std::end(closed_list); it++) {
                _socks.erase(*it);
            }
        }
    }
}

void tcp_server::stop_all()
{
    for (std::vector<socket_t>::iterator it = std::begin(_servers); it != std::end(_servers); ++it) {
        closesocket(*it);
    }

    for (map_sockets::iterator it = std::begin(_socks); it != std::end(_socks); ++it) {
        _handler->on_close(*(it->second));
        delete it->second;
        closesocket(it->first);
    }

    _running = false;
    for (std::deque<thread_ptr>::iterator it = std::begin(_threads); it != std::end(_threads); ++it) {
        if ((*it)->joinable()) {
            (*it)->join();
        }

        delete (*it);
    }

    
    _servers.clear();
    _socks.clear();
    _threads.clear();
}
