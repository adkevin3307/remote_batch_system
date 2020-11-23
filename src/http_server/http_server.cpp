#include <iostream>
#include <boost/asio.hpp>

#include "http_server/Parser.h"

using namespace std;
using namespace boost;

int main(int argc, char** argv)
{
    if (argc != 2) {
        cerr << "Usage: ./http_server {port}" << '\n';

        return 0;
    }

    asio::io_service io_service;
}