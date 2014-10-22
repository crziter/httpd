#ifndef __port_h__
#define __port_h__

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>

#define IF_WINDOWS(x) x
#define CONFIG_FILE "conf\\httpd.conf"
#define MIME_FILE "conf\\mime.types"
#define ERROR_400 "errors\\400.html"
#define ERROR_404 "errors\\404.html"

typedef int socklen_t;
typedef SOCKET socket_t;
#elif defined(__linux__) || defined(__APPLE__)
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#if defined(__APPLE__)
#define CONFIG_FILE "conf/httpd.conf"
#elif defined(__linux__)
#endif

#define IF_WINDOWS(x)
#define INVALID_SOCKET (0)
#define SOCKET_ERROR (-1)

#define ERROR_400 "errors/400.html"
#define ERROR_404 "errors/404.html"
#define MIME_FILE "conf/mime.types"

typedef int socket_t;
#define closesocket(fd) close(fd)
#define _stricmp(x,y) strcasecmp(x,y)
#else
#error Not supported platform
#endif

#include <iostream>
#include <vector>

typedef char * char_ptr;
typedef unsigned short ushort;
typedef std::vector<char> vector_char;

#if defined(DEBUG) || defined(_DEBUG)
#define IF_DEBUG(x) x
#else
#define IF_DEBUG(x)
#endif

#define TWO_NEWLINE "(\\r\\n\\r\\n|\\n\\n)"
#define NEWLINE "(\\\r\\n|\\n)"
#define BLOCK(x) x

#endif // __port_h__