#include "http_server/ServerSession.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <boost/filesystem.hpp>

#ifdef WINDOWS
#include <exception>
#include <boost/asio.hpp>

#include "panel_cgi/Panel.h"
#include "console_cgi/Client.h"
#endif

using namespace std;

ServerSession::ServerSession(shared_ptr<boost::asio::io_context> io_context, boost::asio::ip::tcp::socket socket)
    : _socket(move(socket))
{
    this->_io_context = shared_ptr<boost::asio::io_context>(io_context);
    this->_buffer.resize(CONSTANT::MAX_BUFFER_SIZE);
}

ServerSession::~ServerSession()
{
    this->_buffer.clear();
    this->_buffer.shrink_to_fit();
}

bool ServerSession::check()
{
    bool result = false;

    result = result || (this->header.find(CONSTANT::REQUEST_HEADER::REQUEST_METHOD) != this->header.end());
    result = result || (this->header.find(CONSTANT::REQUEST_HEADER::REQUEST_URI) != this->header.end());
    result = result || (this->header.find(CONSTANT::REQUEST_HEADER::QUERY_STRING) != this->header.end());
    result = result || (this->header.find(CONSTANT::REQUEST_HEADER::SERVER_PROTOCOL) != this->header.end());
    result = result || (this->header.find(CONSTANT::REQUEST_HEADER::HTTP_HOST) != this->header.end());
    result = result || (this->header.find(CONSTANT::REQUEST_HEADER::SERVER_ADDR) != this->header.end());
    result = result || (this->header.find(CONSTANT::REQUEST_HEADER::SERVER_PORT) != this->header.end());
    result = result || (this->header.find(CONSTANT::REQUEST_HEADER::REMOTE_ADDR) != this->header.end());
    result = result || (this->header.find(CONSTANT::REQUEST_HEADER::REMOTE_PORT) != this->header.end());

    return result;
}

void ServerSession::do_read()
{
    auto self(shared_from_this());

    auto handle_buffer = boost::asio::buffer(this->_buffer, this->_buffer.size());
    this->_socket.async_read_some(handle_buffer, [this, self](boost::system::error_code error_code, size_t bytes) {
        if (!error_code) {
            this->parser.parse(this->_socket, this->_buffer.data(), this->header);

            if (this->check()) {
                this->handle_request();
            }

            this->do_read();
        }
        else if (error_code != boost::asio::error::eof && error_code != boost::asio::error::operation_aborted) {
            cerr << "ServerSession read error: " << error_code.message() << '\n';
        }
    });
}

void ServerSession::handle_request()
{
    auto self(shared_from_this());

    string response_header = "";
    boost::filesystem::path exec_file = boost::filesystem::current_path() / this->header[CONSTANT::REQUEST_HEADER::REQUEST_URI];

#ifdef WINDOWS
    if (this->header[CONSTANT::REQUEST_HEADER::REQUEST_URI] == "/panel.cgi" || this->header[CONSTANT::REQUEST_HEADER::REQUEST_URI] == "/console.cgi") {
        response_header += "HTTP/1.1 200 OK\r\n";
    }
    else {
        response_header += "HTTP/1.1 404 Not Found\r\n";
        response_header += "Content-Length: 20\r\n";
        response_header += "<h1>Not Found</h1>\r\n";
    }
#else
    if (boost::filesystem::is_regular_file(exec_file)) {
        response_header += "HTTP/1.1 200 OK\r\n";
    }
    else {
        response_header += "HTTP/1.1 404 Not Found\r\n";
        response_header += "Content-Length: 20\r\n";
        response_header += "<h1>Not Found</h1>\r\n";
    }
#endif

    auto handle_buffer = boost::asio::buffer(response_header, response_header.length());
    this->_socket.async_send(handle_buffer, [this, self, exec_file](boost::system::error_code error_code, size_t bytes) {
        if (!error_code) {
#ifdef WINDOWS
            if (this->header[CONSTANT::REQUEST_HEADER::REQUEST_URI] == "/panel.cgi") {
                Panel panel;

                string response = panel.html_response();
                this->_socket.async_send(boost::asio::buffer(response, response.length()), [](boost::system::error_code error_code, size_t bytes) {
                    if (error_code) {
                        cerr << error_code.message() << '\n';
                    }
                });

                string context = panel.html_template();
                this->_socket.async_send(boost::asio::buffer(context, context.length()), [](boost::system::error_code error_code, size_t bytes) {
                    if (error_code) {
                        cerr << error_code.message() << '\n';
                    }
                });
            }
            else if (this->header[CONSTANT::REQUEST_HEADER::REQUEST_URI] == "/console.cgi") {
                try {
                    shared_ptr<boost::asio::io_context> io_context(new boost::asio::io_context);

                    Client client(io_context, this->header[CONSTANT::REQUEST_HEADER::QUERY_STRING]);

                    io_context->run();
                }
                catch (exception& error) {
                    cerr << "Console cgi exception: " << error.what() << '\n';
                }
            }
#else
            setenv("REQUEST_METHOD", this->header[CONSTANT::REQUEST_HEADER::REQUEST_METHOD].c_str(), 1);
            setenv("REQUEST_URI", this->header[CONSTANT::REQUEST_HEADER::REQUEST_URI].c_str(), 1);
            setenv("QUERY_STRING", this->header[CONSTANT::REQUEST_HEADER::QUERY_STRING].c_str(), 1);
            setenv("SERVER_PROTOCOL", this->header[CONSTANT::REQUEST_HEADER::SERVER_PROTOCOL].c_str(), 1);
            setenv("HTTP_HOST", this->header[CONSTANT::REQUEST_HEADER::HTTP_HOST].c_str(), 1);
            setenv("SERVER_ADDR", this->header[CONSTANT::REQUEST_HEADER::SERVER_ADDR].c_str(), 1);
            setenv("SERVER_PORT", this->header[CONSTANT::REQUEST_HEADER::SERVER_PORT].c_str(), 1);
            setenv("REMOTE_ADDR", this->header[CONSTANT::REQUEST_HEADER::REMOTE_ADDR].c_str(), 1);
            setenv("REMOTE_PORT", this->header[CONSTANT::REQUEST_HEADER::REMOTE_PORT].c_str(), 1);

            this->_io_context->notify_fork(boost::asio::io_context::fork_prepare);
            if (fork() == 0) {
                this->_io_context->notify_fork(boost::asio::io_context::fork_child);

                int socket_fd = this->_socket.native_handle();
                dup2(socket_fd, STDIN_FILENO);
                dup2(socket_fd, STDOUT_FILENO);
                dup2(socket_fd, STDERR_FILENO);

                this->_socket.close();

                if (execlp(exec_file.c_str(), exec_file.c_str(), NULL) < 0) {
                    cout << "<h1>Execution Failed</h1>\r\n";
                }
            }
            else {
                this->_io_context->notify_fork(boost::asio::io_context::fork_parent);

                this->_socket.close();
            }
#endif
        }
        else if (error_code != boost::asio::error::operation_aborted) {
            cerr << "ServerSession handle error: " << error_code.message() << '\n';
        }
    });
}

void ServerSession::start()
{
    this->do_read();
}
