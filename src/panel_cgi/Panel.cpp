#include "panel_cgi/Panel.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "constant.h"

using namespace std;

Panel::Panel(int servers, int host_amount, string testcase_path, string domain)
{
    this->servers = servers;
    this->host_amount = host_amount;
    this->testcase_path = testcase_path;
    this->domain = domain;

    for (auto i = 0; i < this->host_amount; i++) {
        string s = "nplinux" + to_string(i + 1);
        this->hosts.push_back(s);
    }

    boost::filesystem::path path(this->testcase_path);

    if (boost::filesystem::is_directory(path)) {
        for (auto it : boost::make_iterator_range(boost::filesystem::directory_iterator(path), {})) {
            this->testcases.push_back(it.path().filename().string());
        }

        sort(this->testcases.begin(), this->testcases.end());
    }

    this->html_template();
}

Panel::~Panel()
{
}

void Panel::html_template()
{
    cout << "Content-type: text/html\r\n\r\n";
    fflush(stdout);

    string s;
    stringstream ss;
    ss << CONSTANT::PANEL_HTML;

    while (getline(ss, s)) {
        boost::trim(s);

        if (s == "CONTENT") {
            for (auto i = 0; i < this->servers; i++) {
                cout << "<tr>";

                cout << "<th scope='row' class='align-middle'>Session " << to_string(i + 1) << "</th>";

                cout << "<td>";

                cout << "<div class='input-group'>";
                cout << "<select name='h" << to_string(i) << "' class='custom-select'>";
                cout << "<option></option>";

                for (auto host : this->hosts) {
                    cout << "<option value='" << host << "." << this->domain << "'>" << host << "</option>";
                }

                cout << "</select>";

                cout << "<div class='input-group-append'>";
                cout << "<span class='input-group-text'>.cs.nctu.edu.tw</span>";
                cout << "</div>";

                cout << "</div>";

                cout << "</td>";

                cout << "<td>" << "<input name='p" << to_string(i) << "' type='text' class='form-control' size='5'>" << "</td>";

                cout << "<td>";
                cout << "<select name='f" << to_string(i) << "' class='custom-select'>";

                cout << "<option></option>";
                for (auto testcase : this->testcases) {
                    cout << "<option value='" << testcase << "'>" << testcase << "</option>";
                }

                cout << "</select>";

                cout << "</td>";

                cout << "</tr>";
            }
        }
        else {
            cout << s << '\n';
        }
    }

    fflush(stdout);
}