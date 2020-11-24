#pragma once

#include <string>

#include "constant.h"

using namespace std;

class MessageHandler {
public:
    MessageHandler();
    ~MessageHandler();

    static void output(string id, string s, CONSTANT::OUTPUT_TYPE type);
};