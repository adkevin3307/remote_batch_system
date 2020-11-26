#include "console_cgi/MessageHandler.h"

#include <iostream>
#include <sstream>
#include <boost/algorithm/string/replace.hpp>

using namespace std;

MessageHandler::MessageHandler()
{
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::html_escape(string& s)
{
    boost::replace_all(s, "&",  "&amp;");
    boost::replace_all(s, "\"", "&quot;");
    boost::replace_all(s, "\'", "&apos;");
    boost::replace_all(s, "<",  "&lt;");
    boost::replace_all(s, ">",  "&gt;");
    boost::replace_all(s, "\n", "&NewLine;");
}
#ifdef WINDOWS
boost::asio::ip::tcp::socket* MessageHandler::_socket = NULL;

void MessageHandler::enable(boost::asio::ip::tcp::socket* socket)
{
    MessageHandler::_socket = socket;
}
#endif
void MessageHandler::output(string id, string s, CONSTANT::OUTPUT_TYPE type)
{
    MessageHandler::html_escape(s);

    stringstream ss;
    switch (type) {
        case CONSTANT::OUTPUT_TYPE::STDOUT:
            ss << "<script>document.getElementById('s" << id << "').innerHTML += '" << s << "'</script>";

            break;
        case CONSTANT::OUTPUT_TYPE::STDERR:
            ss << "<script>document.getElementById('s" << id << "').innerHTML += '<red>" << s << "</red>'</script>";

            break;
        case CONSTANT::OUTPUT_TYPE::COMMAND:
            ss << "<script>document.getElementById('s" << id << "').innerHTML += '<green>" << s << "</green>'</script>";

            break;
        default:
            break;
    }

#ifdef WINDOWS
    string buffer = ss.str();
    auto handle_buffer = boost::asio::buffer(buffer, buffer.length());
    boost::asio::async_write(*(MessageHandler::_socket), handle_buffer, [](boost::system::error_code error_code, size_t bytes) {
        if (error_code) {
            cerr << "Message handler error: " << error_code.message() << '\n';
        }
    });
#else
    cout << buffer.str();
    fflush(stdout);
#endif
}
