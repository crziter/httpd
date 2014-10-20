#include <iostream>
#include <vector>
#include "tcp_server.h"
#include "socket_wrapper.h"

#if defined(WIN32) || defined(_WIN32)
#pragma comment(lib, "ws2_32.lib")
#endif

tcp_server::tcp_server(tcp_handler_ptr handler)
{
    _handler = handler;
    _running = true;
    _servers = INVALID_SOCKET;
}

tcp_server::~tcp_server()
{
    stop_all();

    for (auto sh : _socks) {
        delete sh.second;
    }

    delete _handler;
}

bool tcp_server::start(const char_ptr addr, ushort port)
{
    if (_servers == INVALID_SOCKET) {
        sockaddr_in info;

        memset(&info, 0, sizeof(info));
        info.sin_addr.s_addr = inet_addr(addr);
        info.sin_family = AF_INET;
        info.sin_port = htons(port);

        _servers = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_servers == INVALID_SOCKET) {
            return false;
        }

        if (::bind(_servers, (struct sockaddr *) &info, sizeof(struct sockaddr)) == SOCKET_ERROR) {
            closesocket(_servers);
            return false;
        }

        if (::listen(_servers, 10) == SOCKET_ERROR) {
            closesocket(_servers);
            return false;
        }

        _threads = std::thread(&tcp_server::process_event, this);
        return true;
    }
    else return false;
}

void tcp_server::process_event()
{
    unsigned int maxfds = 0;
    fd_set readset;

    timeval tv;
    tv.tv_usec = 50000;
    tv.tv_sec = 0;
    while (_running) {
        FD_ZERO(&readset);
        FD_SET(_servers, &readset);

        maxfds = _servers;
        for (auto sm : _socks) {
            if (maxfds < sm.first) maxfds = sm.first;
            FD_SET(sm.first, &readset);
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
            
            /* IF_DEBUG({
                std::cout << "select() time out!" << std::endl;
            }); */
        }
        else {
            if (FD_ISSET(_servers, &readset)) {
                sockaddr_in info;
                socklen_t len = sizeof(struct sockaddr);

                socket_t client = ::accept(_servers, (struct sockaddr *) &info, &len);

                if (client == INVALID_SOCKET) {
                    // error
                    IF_DEBUG({
                        std::cout << "Accept new client error!" << std::endl;
                    });
                }
                else 
                {
                    maxfds = maxfds > client ? maxfds : client;

                    _socks[client] = new tcp_socket(client, inet_ntoa(info.sin_addr), ntohs(info.sin_port));
                    _handler->on_accept(*_socks.at(client));
                }

                FD_CLR(_servers, &readset);
            }

            std::vector<socket_t> closed_list;
            for (auto s : _socks) {
                if (FD_ISSET(s.first, &readset)) {
                    
                    vector_char buffer;
                    socket_wrapper sock(*s.second);
                    int len = sock.recv(buffer);
                    
                    if (len <= 0) {
                        _handler->on_close(*(s.second));
                        closed_list.push_back(s.first);
                        closesocket(s.first);
                    }
                    else if (len > 0)
                        _handler->on_receive(*(s.second), buffer);
                }
            }

            for (auto c : closed_list) {
                _socks.erase(c);
            }
        }
    }
}

void tcp_server::stop_all()
{
    closesocket(_servers);

    for (auto s : _socks) {
        _handler->on_close(*(s.second));
        delete s.second;
        closesocket(s.first);
    }

    _running = false;
    if (_threads.joinable())
        _threads.join();
    
    _socks.clear();
}
