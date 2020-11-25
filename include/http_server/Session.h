#pragma once

#include <map>
#include <vector>
#include <string>
#include <boost/asio.hpp>

#include "constant.h"
#include "http_server/Parser.h"

using namespace std;

class Session : public enable_shared_from_this<Session> {
private:
    shared_ptr<boost::asio::io_context> _io_context;
    boost::asio::ip::tcp::socket _socket;
    vector<char> _buffer;
    Parser parser;
    map<CONSTANT::REQUEST_HEADER, string> header;

    bool check();
    void do_read();
    void handle_request();

public:
    Session(shared_ptr<boost::asio::io_context> io_context, boost::asio::ip::tcp::socket socket);
    ~Session();

    void start();
};