#include "http_server/Server.h"

#include <iostream>
#include <sys/wait.h>

#include "http_server/Session.h"

using namespace std;

Server::Server(shared_ptr<boost::asio::io_context> io_context, int port)
    : _signal(*io_context, SIGCHLD),
      _acceptor(*io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      _socket(*io_context)
{
    this->_io_context = shared_ptr<boost::asio::io_context>(io_context);

    this->do_wait();
    this->do_accept();
}

Server::~Server()
{
}

void Server::do_wait()
{
    this->_signal.async_wait([this](const boost::system::error_code& error_code, int signo) {
        if (this->_acceptor.is_open()) {
            pid_t wpid;

            while (true) {
                wpid = waitpid(-1, NULL, WNOHANG);

                if (wpid == -1) break;
            }

            this->do_wait();
        }
    });
}

void Server::do_accept()
{
    this->_acceptor.async_accept(this->_socket, [this](boost::system::error_code error_code) {
        if (!this->_acceptor.is_open()) return;

        if (!error_code) {
            make_shared<Session>(this->_io_context, move(this->_socket))->start();
        }
        else {
            cerr << "Accept error: " << error_code.message() << '\n';
        }

        this->do_accept();
    });
}