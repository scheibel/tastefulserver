#include <QCoreApplication>

#include <QBuffer>

#include <tastefulserver/HttpServer.h>

using namespace tastefulserver;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    HttpServer server([](const HttpRequest & request) {
            HttpResponse response(http::OK, request);

            response.setStatusCode(http::OK);

            QByteArray content;
            QBuffer buffer(&content);

            request.writeTo(buffer);
            content.append("Client IP:" + request.address().toString() + " (Port " + QString::number(request.port()) + ")\r\n");

            response.setContent(content);

            return response;
        });

    server.listen(QHostAddress::Any, 8080);

    return app.exec();
}
