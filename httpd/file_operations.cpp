#include <iostream>
#include <fstream>
#include "file_operations.h"

#if defined(WIN32) || defined(_WIN32)
#include <direct.h>
#define SPLASH "\\"
#else
#include <unistd.h>
#define _getcwd(x,y) getcwd(x,y)
#define SPLASH "/"
#endif

using std::fstream;
using std::ifstream;
using std::ios;

bool file_operations::exist(std::string& path)
{
    ifstream f;

    f.open(path, ios::in);
    if (f.is_open()) {
        f.close();
        return true;
    }

    return false;
}

bool file_operations::get_content(std::string& path, std::string& content)
{
    if (!exist(path)) return false;

    ifstream ifs;
    ifs.open(path, std::ios::in | std::ios::binary);
    if (ifs.is_open()) {
        content = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
        ifs.close();
    }

    return true;
}

std::string file_operations::jail_path(std::string& path)
{
    char dir_buff[1024];
    if (!_getcwd(dir_buff, sizeof(dir_buff)))
        return path;

    std::string real_path = std::string(dir_buff) + SPLASH + path;
    return real_path;
}

bool file_operations::exist_jail(std::string& path)
{
    std::string rpath = jail_path(path);
    return exist(rpath);
}

bool file_operations::get_content_jail(std::string& path, std::string& content)
{
    std::string rpath = jail_path(path);
    return get_content(rpath, content);
}

std::string file_operations::convert_path(std::string& path)
{
#if defined(WIN32) || defined(_WIN32)
    std::string tmp = path;

    for(int i=0; i<tmp.length(); ++i) {
        if (tmp[i] == '/') {
            tmp[i] = '\\';
        }
    }

    return tmp;
#else
    return path;
#endif
}


