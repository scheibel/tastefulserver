#include "Server.h"

#include <QTimer>

namespace {

QString page = R"(
<html>
<head>

<script type="text/javascript">

output = function(d) {
    var c = document.getElementById('c');
    var e = document.createElement('div');
    e.innerHTML = d;
    c.appendChild(e);
};

window.onload = function() {
    output("test");

    var connection = new WebSocket('ws://localhost:8080/echo', ['soap', 'xmpp']);

    connection.onopen = function () {
        connection.send('Ping');
        var arr = "";
        for (var i = 0; i<300; ++i)
        {
            arr += i.toString()+",";
        }
        arr+="300";

        connection.send(arr);

        var binary = new Uint8Array(8);

        for (var i = 0; i < 8; i++) {
            binary[i] = i;
        }
        connection.send(binary.buffer);
    };

    connection.onerror = function (error) {
        output("connection error");
    };

    var i = 0;

    connection.onmessage = function (e) {
        output(e.data);

        if (i++ > 11)
            connection.close();
    };
}
</script>

</head>
<body>
    <h1>hallo</h2>

    <div id="c">
    </div>
</body>
</html>
)";

}

Server::Server()
{
}

Server::~Server()
{
}

void Server::handleRequest(tastefulserver::HttpSocket * socket, const tastefulserver::HttpRequest & request)
{
    tastefulserver::HttpResponse response(tastefulserver::http::OK, request);

    response.setContent(page.toUtf8());

    socket->send(response);
}

void Server::handleText(tastefulserver::WebSocket * socket, const QByteArray & text)
{
    socket->sendText(text);

    socket->sendPing();
}

void Server::handleBinary(tastefulserver::WebSocket * socket, const QByteArray & binary)
{
    socket->sendBinary(binary);
}

void Server::connectionEstablished(tastefulserver::WebSocket * socket)
{
    QTimer * timer = new QTimer(socket);
    timer->setInterval(1000);

    connect(timer, &QTimer::timeout, [socket]() {
        QString text = "hallo "+QString::number(qrand());

        qDebug() << "Send:" << text;
        socket->sendText(text.toLatin1());
    });

    timer->start();
}
