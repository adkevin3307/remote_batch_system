#pragma once

#include <vector>
#include <string>

using namespace std;

class Panel {
private:
    int servers, host_amount;
    string domain, testcase_path;
    vector<string> hosts;
    vector<string> testcases;

public:
    Panel(int servers = 5, int host_amount = 12, string testcase_path = "test_case", string domain = "cs.nctu.edu.tw");
    ~Panel();

    string html_response();
    string html_template();
};