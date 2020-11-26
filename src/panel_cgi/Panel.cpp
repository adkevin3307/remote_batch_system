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
}

Panel::~Panel()
{
}

string Panel::html_response()
{
    string s = "Content-type: text/html\r\n\r\n";

    return s;
}

string Panel::html_template()
{
    string s;
    stringstream ss, result;
    ss << CONSTANT::PANEL_HTML;

    while (getline(ss, s)) {
        boost::trim(s);

        if (s == "CONTENT") {
            for (auto i = 0; i < this->servers; i++) {
                result << "<tr>";

                result << "<th scope='row' class='align-middle'>Session " << to_string(i + 1) << "</th>";

                result << "<td>";

                result << "<div class='input-group'>";
                result << "<select name='h" << to_string(i) << "' class='custom-select'>";
                result << "<option></option>";

                for (auto host : this->hosts) {
                    result << "<option value='" << host << "." << this->domain << "'>" << host << "</option>";
                }

                result << "</select>";

                result << "<div class='input-group-append'>";
                result << "<span class='input-group-text'>.cs.nctu.edu.tw</span>";
                result << "</div>";

                result << "</div>";

                result << "</td>";

                result << "<td>" << "<input name='p" << to_string(i) << "' type='text' class='form-control' size='5'>" << "</td>";

                result << "<td>";
                result << "<select name='f" << to_string(i) << "' class='custom-select'>";

                result << "<option></option>";
                for (auto testcase : this->testcases) {
                    result << "<option value='" << testcase << "'>" << testcase << "</option>";
                }

                result << "</select>";

                result << "</td>";

                result << "</tr>";
            }
        }
        else {
            result << s << '\n';
        }
    }

    return result.str();
}