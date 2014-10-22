#ifndef __file_operations_h__
#define __file_operations_h__

#include <iostream>
#include <string>

class file_operations
{
public:
    static bool exist(std::string& path);
    static bool get_content(std::string& path, std::string& content);

    static bool exist_jail(std::string& path);
    static bool get_content_jail(std::string& path, std::string& content);

    static std::string convert_path(std::string& path);

private:
    static std::string jail_path(std::string& path);
};

#endif