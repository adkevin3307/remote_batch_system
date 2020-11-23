#include "http_server/Parser.h"

#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <iostream>

using namespace std;

Parser::Parser()
{
}

Parser::~Parser()
{
}

bool Parser::is_method(string s)
{
    vector<string> methods{ "GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH" };

    string s_copy = boost::to_upper_copy(s);

    return (find(methods.begin(), methods.end(), s_copy) != methods.end());
}

map<CONSTANT::REQUEST_HEADER, string> Parser::parse(boost::asio::ip::tcp::socket socket, string s)
{
    boost::trim(s);

    vector<string> tokens;
    split(tokens, s, boost::is_any_of(" "), boost::token_compress_on);

    map<CONSTANT::REQUEST_HEADER, string> result;

    if (tokens.empty()) return result;
    else if (this->is_method(tokens.front())) {
        result[CONSTANT::REQUEST_HEADER::REQUEST_METHOD] = tokens[0];

        auto it = tokens[1].find_first_of('?');
        result[CONSTANT::REQUEST_HEADER::REQUEST_URI] = tokens[1].substr(0, it);
        result[CONSTANT::REQUEST_HEADER::QUERY_STRING] = tokens[1].substr(it + 1);

        result[CONSTANT::REQUEST_HEADER::SERVER_PROTOCOL] = tokens[2];
    }
    else if (tokens.front() == "Host:") {
        result[CONSTANT::REQUEST_HEADER::HTTP_HOST] = tokens[2];
    }

    result[CONSTANT::REQUEST_HEADER::SERVER_ADDR] = socket.local_endpoint().address().to_string();
    result[CONSTANT::REQUEST_HEADER::SERVER_PORT] = to_string(socket.local_endpoint().port());

    result[CONSTANT::REQUEST_HEADER::REMOTE_ADDR] = socket.remote_endpoint().address().to_string();
    result[CONSTANT::REQUEST_HEADER::REMOTE_PORT] = to_string(socket.remote_endpoint().port());

    return result;
}