#ifndef __port_h__
#define __port_h__

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>

#define IF_WINDOWS(x) x
#define CONFIG_FILE "\\\\psf\\Home\\Desktop\\final\\httpd\\conf\\httpd.conf"

typedef int socklen_t;
typedef SOCKET socket_t;
#elif defined(__linux__) || defined(__APPLE__)
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#if defined(__APPLE__)
#define CONFIG_FILE "/Users/binhtruong/Desktop/final/httpd/conf/httpd.conf"
#elif defined(__linux__)
#endif

#define IF_WINDOWS(x)
#define INVALID_SOCKET (0)
#define SOCKET_ERROR (-1)

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

#define IF_DEBUG(x) x
#define TWO_NEWLINE "(?:\\r\\n\\r\\n|\\n\\n)"
#define NEWLINE "(?:\\\r\\n|\\n)"

#endif // __port_h__