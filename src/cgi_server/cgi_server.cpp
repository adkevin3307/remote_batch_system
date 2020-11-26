#include <iostream>
#include <exception>
#include <boost/asio.hpp>

#include "http_server/Server.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2) {
        cerr << "Usage: ./cgi_server.exe {port}" << '\n';

        return 0;
    }

    try {
        shared_ptr<boost::asio::io_context> io_context(new boost::asio::io_context);

        Server server(io_context, atoi(argv[1]));

        io_context->run();
    }
    catch (exception& error) {
        cerr << "Cgi server exception: " << error.what() << '\n';
    }

    return 0;
}
