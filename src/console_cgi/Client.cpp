#include "console_cgi/Client.h"

#include <iostream>
#include <vector>
#include <boost/algorithm/string/trim.hpp>

#include "constant.h"
#include "console_cgi/Parser.h"

using namespace std;

Client::Client(shared_ptr<boost::asio::io_context> io_context)
{
    this->_io_context = shared_ptr<boost::asio::io_context>(io_context);

    this->keys = vector<string>{ "h", "p", "f" };
    this->information = Parser::parse();

    this->html_template();
    this->execute_sessions();
}

Client::~Client()
{
}

void Client::html_template()
{
    cout << "Content-type: text/html\r\n\r\n";
    fflush(stdout);

    string s;
    int connections = 0;

    stringstream ss;
    ss << CONSTANT::CONSOLE_HTML;

    while (getline(ss, s)) {
        boost::trim(s);

        if (s == "TITLE") {
            cout << "<tr>" << '\n';

            for (size_t i = 0; i < this->information.size() / this->keys.size(); i++) {
                string host = this->information[this->keys[0] + to_string(i)];
                string port = this->information[this->keys[1] + to_string(i)];

                if (host != "" && port != "") {
                    connections += 1;

                    cout << "<th scope='col'>" << '\n';
                    cout << host << ':' << port << '\n';
                    cout << "</th>" << '\n';
                }
            }

            cout << "</tr>" << '\n';
        }
        else if (s == "CONTENT") {
            cout << "<tr>" << '\n';

            for (auto i = 0; i < connections; i++) {
                cout << "<td><pre id='s" << i << "' class='mb-0'></pre></td>" << '\n';
            }

            cout << "</tr>" << '\n';
        }
        else {
            cout << s << '\n';
        }
    }

    fflush(stdout);
}

void Client::execute_sessions()
{
    for (size_t i = 0; i < this->information.size() / this->keys.size(); i++) {
        string host = this->information[keys[0] + to_string(i)];
        string port = this->information[keys[1] + to_string(i)];
        string filename = this->information[keys[2] + to_string(i)];

        if (host != "" && port != "" && filename != "") {
            auto ptr = make_shared<Session>(this->_io_context, i, host, port, filename);

            this->sessions.push_back(ptr);
            ptr->start();
        }
    }
}