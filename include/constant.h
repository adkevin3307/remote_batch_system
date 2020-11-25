#pragma once

#include <string>

using namespace std;

namespace CONSTANT {
    const static size_t MAX_BUFFER_SIZE = 4096;

    enum REQUEST_HEADER {
        REQUEST_METHOD,
        REQUEST_URI,
        QUERY_STRING,
        SERVER_PROTOCOL,
        HTTP_HOST,
        SERVER_ADDR,
        SERVER_PORT,
        REMOTE_ADDR,
        REMOTE_PORT,
    };

    enum OUTPUT_TYPE {
        STDOUT,
        STDERR,
        COMMAND,
    };

    const static string CONSOLE_HTML = R"(
        <!DOCTYPE html>
        <html lang='en'>

        <head>
            <meta charset='UTF-8'>
            <title>NP Project 3 Console</title>
            <link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css' integrity='sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2' crossorigin='anonymous'>
            <link href='https://fonts.googleapis.com/css?family=Source+Code+Pro' rel='stylesheet'>
            <link rel='icon' type='image/png' href='https://cdn0.iconfinder.com/data/icons/small-n-flat/24/678068-terminal-512.png'>
            <style>
                * {
                    font-family: 'Source Code Pro', monospace;
                    font-size: 1rem !important;
                }

                body {
                    background-color: #212529;
                }

                pre {
                    color: #cccccc;
                }

                green {
                    color: #01b468;
                }

                red {
                    color: red;
                }
            </style>
        </head>

        <body>
            <table class='table table-dark table-bordered'>
                <thead>
                    TITLE
                </thead>
                <tbody>
                    CONTENT
                </tbody>
            </table>
        </body>

        </html>
    )";

    const static string PANEL_HTML = R"(
        <!DOCTYPE html>
        <html lang='en'>

        <head>
            <title>NP Project 3 Panel</title>
            <link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css' integrity='sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2' crossorigin='anonymous'>
            <link href='https://fonts.googleapis.com/css?family=Source+Code+Pro' rel='stylesheet'>
            <link rel='icon' type='image/png' href='https://cdn4.iconfinder.com/data/icons/iconsimple-setting-time/512/dashboard-512.png'>
            <style>
                * {
                    font-family: 'Source Code Pro', monospace;
                }
            </style>
        </head>
        <body class='bg-secondary pt-5'>
            <form action='console.cgi' method='GET'>
                <table class='table mx-auto bg-light' style='width: inherit'>
                    <thead class='thead-dark'>
                        <tr>
                            <th scope='col'>#</th>
                            <th scope='col'>Host</th>
                            <th scope='col'>Port</th>
                            <th scope='col'>Input File</th>
                        </tr>
                    </thead>
                    <tbody>
                        CONTENT
                        <tr>
                            <td colspan='3'></td>
                            <td><button type='submit' class='btn btn-info btn-block'>Run</button></td>
                        </tr>
                    </tbody>
                </table>
            </form>
        </body>

        </html>
    )";
};