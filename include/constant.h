#pragma once

namespace CONSTANT {
    const static size_t MAX_BUFFER_SIZE = 4096;

    enum REQUEST_HEADER {
        REQUEST_METHOD,
        REQUEST_URI,
        QUERY_STRING,
        SERVER_PROTOCOL,
        HTTP_HOST,
        SERVER_ADDR,
        SERVER_PORT,
        REMOTE_ADDR,
        REMOTE_PORT,
    };
};