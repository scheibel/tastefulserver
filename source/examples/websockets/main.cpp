#include <QCoreApplication>
#include <QString>

#include <tastefulserver/HttpServer.h>

using namespace tastefulserver;

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
