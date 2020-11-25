#pragma once

#include <map>
#include <string>

using namespace std;

class Parser {
public:
    Parser();
    ~Parser();

    static map<string, string> parse();
};