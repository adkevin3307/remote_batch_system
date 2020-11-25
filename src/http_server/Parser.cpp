#include "http_server/Parser.h"

#include <vector>
#include <sstream>
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

void Parser::parse(boost::asio::ip::tcp::socket& socket, string s, map<CONSTANT::REQUEST_HEADER, string>& header)
{
    boost::trim(s);

    stringstream ss;
    ss << s;

    string line;
    while (getline(ss, line)) {
        vector<string> tokens;
        split(tokens, line, boost::is_any_of(" "), boost::token_compress_on);

        if (tokens.empty()) return;
        else if (this->is_method(tokens.front())) {
            header[CONSTANT::REQUEST_HEADER::REQUEST_METHOD] = tokens[0];

            auto it = tokens[1].find_first_of('?');
            header[CONSTANT::REQUEST_HEADER::REQUEST_URI] = tokens[1].substr(0, it);
            header[CONSTANT::REQUEST_HEADER::QUERY_STRING] = (it == string::npos ? "" : tokens[1].substr(it + 1));

            header[CONSTANT::REQUEST_HEADER::SERVER_PROTOCOL] = tokens[2];
        }
        else if (tokens.front() == "Host:") {
            header[CONSTANT::REQUEST_HEADER::HTTP_HOST] = tokens[1];
        }
    }

    header[CONSTANT::REQUEST_HEADER::SERVER_ADDR] = socket.local_endpoint().address().to_string();
    header[CONSTANT::REQUEST_HEADER::SERVER_PORT] = to_string(socket.local_endpoint().port());

    header[CONSTANT::REQUEST_HEADER::REMOTE_ADDR] = socket.remote_endpoint().address().to_string();
    header[CONSTANT::REQUEST_HEADER::REMOTE_PORT] = to_string(socket.remote_endpoint().port());

    return;
}