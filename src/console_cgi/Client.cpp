#include "console_cgi/Client.h"

#include <boost/filesystem.hpp>

#include "constant.h"
#include "console_cgi/MessageHandler.h"

using namespace std;

Client::Client(boost::asio::io_context& io_context, string host, string port, string filename, int id)
    : host(host), port(port), filename(filename), _socket(io_context), _resolver(io_context)
{
    this->id = to_string(id);

    this->index = 0;
    this->_buffer.resize(CONSTANT::MAX_BUFFER_SIZE);

    boost::filesystem::path test_filename = boost::filesystem::path("test_case") / this->filename;

    if (boost::filesystem::is_regular_file(test_filename)) {
        string line;
        boost::filesystem::fstream file(test_filename);

        while (getline(file, line)) {
            this->commands.push_back(line);
        }

        file.close();
    }
}

Client::~Client()
{
}

void Client::resolve_host()
{
    boost::asio::ip::tcp::resolver::query query(this->host, this->port);
    this->_resolver.async_resolve(query, [this](const boost::system::error_code& error_code, boost::asio::ip::tcp::resolver::iterator it) {
        if (!error_code) {
            this->connect_host(it);
        }
        else {
            string error_message = "Resolve error: " + error_code.message() + '\n';
            MessageHandler::output(this->id, error_message, CONSTANT::OUTPUT_TYPE::STDERR);
        }
    });
}

void Client::connect_host(boost::asio::ip::tcp::resolver::iterator it)
{
    this->_socket.async_connect(it->endpoint(), [this](const boost::system::error_code& error_code) {
        if (!error_code) {
            this->do_read();
        }
        else {
            string error_message = "Connect error: " + error_code.message() + '\n';
            MessageHandler::output(this->id, error_message, CONSTANT::OUTPUT_TYPE::STDERR);
        }
    });
}

void Client::do_read()
{
    auto handle_buffer = boost::asio::buffer(this->_buffer, this->_buffer.size());
    this->_socket.async_read_some(handle_buffer, [this](const boost::system::error_code& error_code, size_t bytes) {
        if (!error_code) {
            if (this->_buffer[0] == '%' && this->_buffer[1] == ' ') {
                this->do_write();
            }
            else {
                string s;
                for (size_t i = 0; i < bytes; i++) {
                    s += this->_buffer[i];
                }
                s += '\n';

                MessageHandler::output(this->id, s, CONSTANT::OUTPUT_TYPE::STDOUT);

                this->do_read();
            }
        }
        else {
            string error_message = "Read error: " + error_code.message() + '\n';
            MessageHandler::output(this->id, error_message, CONSTANT::OUTPUT_TYPE::STDERR);
        }
    });
}

void Client::do_write()
{
    string command = this->commands[this->index++];

    if (command.empty()) {
        MessageHandler::output(this->id, "% \n", CONSTANT::OUTPUT_TYPE::STDOUT);
    }
    else {
        auto handle_buffer = boost::asio::buffer(command + '\n', command.size() + 1);
        this->_socket.async_send(handle_buffer, [this](boost::system::error_code error_code, size_t bytes) {
            if (!error_code) {
                MessageHandler::output(this->id, "% \n", CONSTANT::OUTPUT_TYPE::STDOUT);

                this->do_read();
            }
            else {
                string error_message = "Write error: " + error_code.message() + '\n';
                MessageHandler::output(this->id, error_message, CONSTANT::OUTPUT_TYPE::STDERR);
            }
        });
    }
}

void Client::start()
{
    this->resolve_host();
}