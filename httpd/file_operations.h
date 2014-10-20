#ifndef __file_operations_h__
#define __file_operations_h__

#include <iostream>
#include <string>

class file_operations
{
public:
    static bool exist(std::string& path);
    static bool get_content(std::string& path, std::string& content);
};

#endif