#include "http_error.h"
#include "file_operations.h"
#include "port.h"

std::string http_error::error(http_status status)
{
    switch (status) {
    case BAD_REQUEST:
        {
            std::string content;
            std::string err404(ERROR_400);
            file_operations::get_content_jail(err404, content);
            return content;
        }
        break;
    }

    return "";
}

