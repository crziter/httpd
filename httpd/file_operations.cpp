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

    ifstream f;
    char *buff = nullptr;
    f.open(path);
    if (f.is_open()) {
        int length;
        f.seekg(0, ios::end);
        length = f.tellg();
        buff = new char[length + 1];
        f.seekg(0, ios::beg);

        f.read(buff, length);
        buff[length] = 0;

        content.clear();
        content.append(buff, length + 1);

        delete[] buff;
        f.close();
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

