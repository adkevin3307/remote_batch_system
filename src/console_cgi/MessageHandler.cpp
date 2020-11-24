#include "console_cgi/MessageHandler.h"

#include <iostream>
#include <boost/algorithm/string/replace.hpp>

using namespace std;

MessageHandler::MessageHandler()
{
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::output(string id, string s, CONSTANT::OUTPUT_TYPE type)
{
    boost::replace_all(s, "\n", "&NewLine;");

    switch (type) {
        case CONSTANT::OUTPUT_TYPE::STDOUT:
            cout << "<script>document.getElementById('s" << id << "').innerHTML += '" << s << "'</script>";
            fflush(stdout);

            break;
        case CONSTANT::OUTPUT_TYPE::STDERR:
            cerr << "<script>document.getElementById('s" << id << "').innerHTML += '<stderr>" << s << "</stderr>'</script>";
            fflush(stderr);

            break;
        case CONSTANT::OUTPUT_TYPE::COMMAND:
            cout << "<script>document.getElementById('s" << id << "').innerHTML += '<command>" << s << "</command>'</script>";
            fflush(stdout);

            break;
        default:
            break;
    }
}
