/**
 * (C) LGPL-3
 *
 * Tasteful Server <https://github.com/scheibel/tasteful-server>
 *
 * Copyright: 2012-2014 Lux, Scheibel
 * Authors:
 *     Roland Lux <rollux2000@googlemail.com>
 *     Willy Scheibel <willyscheibel@gmx.de>
 *
 * This file is part of Tasteful Server.
 *
 * Tasteful Server is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tasteful Server is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Tasteful Server.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <QCoreApplication>
#include <QString>

#include <tastefulserver/HttpServer.h>

using namespace tastefulserver;

QString page = R"(
<html>
<head>
</head>
<body>
        <h1>hallo</h2>
        <script>

var connection = new WebSocket('ws://localhost:8080/echo', ['soap', 'xmpp']);

document.writeln("foo");

// When the connection is open, send some data to the server
connection.onopen = function () {
  connection.send('Ping'); // Send the message 'Ping' to the server

        var arr = "";
        for (var i = 0; i<=300; ++i)
        {
            arr += i.toString()+",";
        }
         connection.send(arr);

         var binary = new Uint8Array(8);
         for (var i = 0; i < 8; i++) {
           binary[i] = i;
         }
         connection.send(binary.buffer);
};

// Log errors
connection.onerror = function (error) {
  document.writeln('WebSocket Error ' + error);
        console.log(error);
};

// Log messages from the server
connection.onmessage = function (e) {
  document.writeln('Server: ' + e.data);
};

        </script>
</body>
</html>
)";

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    HttpServer server([](const HttpRequest & request) {
            HttpResponse response(http::OK, request);

            response.setContent(page.toUtf8());

            return response;
        });

    server.listen(QHostAddress::Any, 8080);

    return app.exec();
}
