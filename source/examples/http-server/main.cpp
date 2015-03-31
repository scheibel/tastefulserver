#include <QCoreApplication>

#include <QBuffer>

#include <tastefulserver/HttpCallbackServer.h>

using namespace tastefulserver;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    HttpCallbackServer server([](const HttpRequest & request) {
            HttpResponse response(http::OK, request);

            response.setStatusCode(http::OK);

            QByteArray content;
            QBuffer buffer(&content);
            buffer.open(QIODevice::ReadWrite);

            request.writeTo(buffer);
            content.append("Client IP:" + request.address().toString() + " (Port " + QString::number(request.port()) + ")\r\n");

            response.setContent(content);

            return response;
        });

    server.listen(QHostAddress::Any, 8080);

    return app.exec();
}
