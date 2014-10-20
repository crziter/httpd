#include <iostream>
#include <fstream>
#include "file_operations.h"

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

