#pragma once

#include <map>
#include <string>
#include <boost/asio.hpp>

#include "constant.h"

using namespace std;

class HeaderParser {
private:
    bool is_method(string s);

public:
    HeaderParser();
    ~HeaderParser();

    void parse(boost::asio::ip::tcp::socket& socket, string s, map<CONSTANT::REQUEST_HEADER, string>& header);
};