#pragma once

#include <boost/asio.hpp>

using namespace std;

class Server {
private:
    boost::asio::signal_set _signal;
    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::ip::tcp::socket _socket;

    void do_wait();
    void do_accept();

public:
    Server(boost::asio::io_context& io_context, int port);
    ~Server();
};