#include "console_cgi/Parser.h"

#include <cstdlib>
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace std;

Parser::Parser()
{
}

Parser::~Parser()
{
}

map<string, string> Parser::parse()
{
    map<string, string> result;
    string target = "QUERY_STRING";

    if (getenv(target.c_str())) {
        vector<string> tokens;
        string s = getenv(target.c_str());

        split(tokens, s, boost::is_any_of("&"), boost::token_compress_on);

        for (auto token : tokens) {
            auto it = token.find_first_of('=');

            string key = token.substr(0, it);
            string value = (it == string::npos ? "" : token.substr(it + 1));

            result[key] = value;
        }
    }

    return result;
}