#include <iostream>
#include <regex>
#include "http_uri.h"

using std::regex;
using std::smatch;
using std::regex_search;
using std::string;

int mg_url_decode(const char *src, int src_len, char *dst,
    int dst_len, bool is_form_url_encoded) {
    int i, j, a, b;
#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

    for (i = j = 0; i < src_len && j < dst_len - 1; i++, j++) {
        if (src[i] == '%' && i < src_len - 2 &&
            isxdigit(*(const unsigned char *)(src + i + 1)) &&
            isxdigit(*(const unsigned char *)(src + i + 2))) {
            a = tolower(*(const unsigned char *)(src + i + 1));
            b = tolower(*(const unsigned char *)(src + i + 2));
            dst[j] = (char)((HEXTOI(a) << 4) | HEXTOI(b));
            i += 2;
        }
        else if (is_form_url_encoded && src[i] == '+') {
            dst[j] = ' ';
        }
        else {
            dst[j] = src[i];
        }
    }

    dst[j] = '\0'; // Null-terminate the destination

    return i >= src_len ? j : -1;
}

void http_uri::parse(std::string& data)
{
    //char buff[1024];

    // mg_url_decode(data.c_str(), data.length(), buff, sizeof(buff), true);
    _resource = data; //std::string(buff);
    string tmp = _resource;

    bool quit = false;
    regex reg_path("([^]+)\\?");
    smatch match_path;
    if (regex_search(_resource, match_path, reg_path)) {
        tmp = match_path.suffix().str();
        std::string path_tmp = match_path[1].str();
        _path = decode(path_tmp);
    } else {
        _path = decode(_resource);
        quit = true;
    }

    auto splash_idx = _path.find_last_of('/');
    if (splash_idx == _path.length() - 1) {
        _path = _path + "index.html";
    } else {
        string file_name = _path.substr(splash_idx);
        if (file_name.find_first_of('.') == string::npos) {
            // Not found . in file name
            _path = _path + "/index.html";
        }
    }

    if (quit) return;

    regex regex_query("([^]+)#");
    smatch match_query;
    if (regex_search(tmp, match_query, regex_query)) {
        tmp = match_query.suffix().str();
        _query = match_query[1].str();
    }
    else {
        _query = tmp;
        return;
    }

    if (tmp.length() > 0) {
        _hash = tmp;
    }
}

std::string& http_uri::path()
{
    return _path;
}

std::string http_uri::decode(std::string& msg)
{
    char buff[2048];
    mg_url_decode(msg.c_str(), msg.length(), buff, sizeof(buff), true);
    return string(buff);
}

std::string& http_uri::query()
{
    return _query;
}

std::string http_uri::file_extension()
{
    regex reg_ext(".([a-zA-Z]+)$");
    smatch match_ext;
    if (regex_search(_path, match_ext, reg_ext)) {
        return match_ext[1].str();
    }

    return "";
}

std::string& http_uri::resource()
{
    return _resource;
}
