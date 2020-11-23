#include "http_server/Session.h"

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <boost/filesystem.hpp>

using namespace std;

Session::Session(shared_ptr<boost::asio::io_context> io_context, boost::asio::ip::tcp::socket socket)
    : _socket(move(socket))
{
    this->_io_context = shared_ptr<boost::asio::io_context>(io_context);
    this->_buffer.resize(CONSTANT::MAX_BUFFER_SIZE);
}

Session::~Session()
{
    this->_buffer.clear();
    this->_buffer.shrink_to_fit();
}

void Session::do_read()
{
    auto self(shared_from_this());

    auto handle_buffer = boost::asio::buffer(this->_buffer, this->_buffer.size());
    this->_socket.async_read_some(handle_buffer, [this, self](boost::system::error_code error_code, size_t bytes) {
        if (!error_code) {
            this->header = this->parser.parse(this->_socket, this->_buffer.data());

            if (!this->header.empty()) {
                this->handle_request();
            }
            
            this->do_read();
        }
    });
}

void Session::handle_request()
{
    auto self(shared_from_this());

    stringstream response_header;
    boost::filesystem::path exec_file = boost::filesystem::current_path() / this->header[CONSTANT::REQUEST_HEADER::REQUEST_URI];

    if (boost::filesystem::is_regular_file(exec_file)) {
        response_header << "HTTP/1.1 200 OK\r\n";
    }
    else {
        response_header << "HTTP/1.1 404 Not Found\r\n" << "Content-Length: 20\r\n" << "\r\n" << "<h1>Not Found</h1>\r\n";
    }

    auto handle_buffer = boost::asio::buffer(response_header.str(), response_header.str().length());
    this->_socket.async_send(handle_buffer, [this, self, exec_file](boost::system::error_code error_code, size_t bytes) {
        if (!error_code) {
            setenv("REQUEST_METHOD", this->header[CONSTANT::REQUEST_HEADER::REQUEST_METHOD].c_str(), 1);
            setenv("REQUEST_URI", this->header[CONSTANT::REQUEST_HEADER::REQUEST_URI].c_str(), 1);
            setenv("QUERY_STRING", this->header[CONSTANT::REQUEST_HEADER::QUERY_STRING].c_str(), 1);
            setenv("SERVER_PROTOCOL", this->header[CONSTANT::REQUEST_HEADER::SERVER_PROTOCOL].c_str(), 1);
            setenv("HTTP_HOST", this->header[CONSTANT::REQUEST_HEADER::HTTP_HOST].c_str(), 1);
            setenv("SERVER_ADDR", this->header[CONSTANT::REQUEST_HEADER::SERVER_ADDR].c_str(), 1);
            setenv("SERVER_PORT", this->header[CONSTANT::REQUEST_HEADER::SERVER_PORT].c_str(), 1);
            setenv("REMOTE_ADDR", this->header[CONSTANT::REQUEST_HEADER::REMOTE_ADDR].c_str(), 1);
            setenv("REMOTE_PORT", this->header[CONSTANT::REQUEST_HEADER::REMOTE_PORT].c_str(), 1);

            (*(this->_io_context)).notify_fork(boost::asio::io_context::fork_prepare);
            if (fork() == 0) {
                (*(this->_io_context)).notify_fork(boost::asio::io_context::fork_child);

                int socket_fd = this->_socket.native_handle();
                dup2(socket_fd, STDIN_FILENO);
                dup2(socket_fd, STDOUT_FILENO);
                dup2(socket_fd, STDERR_FILENO);

                close(socket_fd);

                if (execlp(exec_file.c_str(), exec_file.c_str(), NULL) < 0) {
                    cout << "<h1>Execution Failed</h1>\r\n";
                }

                boost::system::error_code trash;
                this->_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, trash);
            }
            else {
                (*(this->_io_context)).notify_fork(boost::asio::io_context::fork_parent);
            }
        }
    });
}

void Session::start()
{
    this->do_read();
}