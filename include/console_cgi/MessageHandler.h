#pragma once

#include <string>
#include <boost/asio.hpp>

#include "constant.h"

using namespace std;

class MessageHandler {
private:
#ifdef WINDOWS
    static boost::asio::ip::tcp::socket* _socket;
#endif
    static void html_escape(string& s);

public:
    MessageHandler();
    ~MessageHandler();

#ifdef WINDOWS
    static void enable(boost::asio::ip::tcp::socket* socket);
#endif
    static void output(string id, string s, CONSTANT::OUTPUT_TYPE type);
};