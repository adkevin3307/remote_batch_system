#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "constant.h"
#include "console_cgi/QueryParser.h"
#include "console_cgi/Client.h"

using namespace std;

void html_template(map<string, string>& information)
{
    cout << "Content-type: text/html\r\n\r\n";
    fflush(stdout);

    string s;
    int connections = 0;
    vector<string> keys{ "h", "p", "f" };

    stringstream ss;
    ss << CONSTANT::CONSOLE_HTML;

    while (getline(ss, s)) {
        boost::trim(s);

        if (s == "TITLE") {
            cout << "<tr>" << '\n';

            for (size_t i = 0; i < information.size() / keys.size(); i++) {
                if (information[keys[0] + to_string(i)] != "" && information[keys[1] + to_string(i)] != "") {
                    connections += 1;

                    cout << "<th scope='col'>" << '\n';
                    cout << information[keys[0] + to_string(i)] << ':' << information[keys[1] + to_string(i)] << '\n';
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

int main()
{
    map<string, string> information = QueryParser::parse();

    html_template(information);

    boost::asio::io_context io_context;

    vector<shared_ptr<Client>> clients;
    vector<string> keys{ "h", "p", "f" };

    for (size_t i = 0; i < information.size() / keys.size(); i++) {
        string host = information[keys[0] + to_string(i)];
        string port = information[keys[1] + to_string(i)];
        string filename = information[keys[2] + to_string(i)];

        if (host != "" && port != "" && filename != "") {
            auto ptr = make_shared<Client>(io_context, host, port, filename, i);

            clients.push_back(ptr);
            ptr->start();
        }
    }

    io_context.run();

    return 0;
}
