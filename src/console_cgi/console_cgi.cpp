#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <boost/algorithm/string/trim.hpp>

#include "console_cgi/QueryParser.h"

using namespace std;

void html_template(map<string, string>& information)
{
    cout << "Content-type: text/html" << "\r\n\r\n";

    fstream file;
    file.open("src/console_cgi/console.html", ios::in);

    string s;
    int connections = 0;
    vector<string> keys{ "h", "p", "f" };

    while (getline(file, s)) {
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

    file.close();
}

int main()
{
    map<string, string> information = QueryParser::parse();

    html_template(information);

    return 0;
}