#pragma once

#include <map>
#include <vector>
#include <string>
#include <boost/asio.hpp>

#include "console_cgi/Session.h"

using namespace std;

class Client {
private:
    vector<string> keys;
    map<string, string> information;
    shared_ptr<boost::asio::io_context> _io_context;
    vector<shared_ptr<Session>> sessions;

    void html_template();
    void execute_sessions();

public:
    Client(shared_ptr<boost::asio::io_context> io_context);
    ~Client();
};