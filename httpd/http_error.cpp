#include "http_error.h"
#include "file_operations.h"
#include "port.h"

std::string http_error::error(http_status status)
{
    std::string content;

    switch (status) {
    case BAD_REQUEST:
        {
            std::string err400(ERROR_400);
            file_operations::get_content_jail(err400, content);
        }
        break;

    case NOT_FOUND:
        {
            std::string err404(ERROR_404);
            file_operations::get_content_jail(err404, content);
        }
        break;
    }

    return content;
}

