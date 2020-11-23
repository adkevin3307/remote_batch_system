#pragma once

#include <map>
#include <string>
#include <boost/asio.hpp>

#include "constant.h"

using namespace std;

class Parser {
private:
    bool is_method(string s);

public:
    Parser();
    ~Parser();

    map<CONSTANT::REQUEST_HEADER, string> parse(boost::asio::ip::tcp::socket socket, string s);
};