#include "http_uri.h"

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

void http_uri::decode(std::string& data)
{
    char buff[1024];

    mg_url_decode(data.c_str(), data.length(), buff, sizeof(buff), true);
    _uri = std::string(buff);
}

std::string& http_uri::uri()
{
    return _uri;
}
