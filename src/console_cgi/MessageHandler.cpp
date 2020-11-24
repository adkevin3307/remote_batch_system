#include "console_cgi/MessageHandler.h"

#include <iostream>

using namespace std;

MessageHandler::MessageHandler()
{
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::output(string id, string s, CONSTANT::OUTPUT_TYPE type)
{
    switch (type) {
        case CONSTANT::OUTPUT_TYPE::STDOUT:
            cout << "<script>document.getElementById('s" << id << "').innerHTML += '" << s << "'</script>";

            break;
        case CONSTANT::OUTPUT_TYPE::STDERR:
            cout << "<script>document.getElementById('s" << id << "').innerHTML += '<stderr>" << s << "</stderr>'</script>";

            break;
        case CONSTANT::OUTPUT_TYPE::COMMAND:
            cout << "<script>document.getElementById('s" << id << "').innerHTML += '<command>" << s << "</command>'</script>";

            break;
        default:
            break;
    }
}