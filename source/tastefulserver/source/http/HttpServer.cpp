#include <tastefulserver/HttpServer.h>
#include <tastefulserver/WebSocket.h>

#include <QTimer>

namespace tastefulserver {

HttpServer::HttpServer(const RequestCallback & callback)
: m_callback(callback)
{
}

HttpServer::~HttpServer()
{
}

AbstractSocket * HttpServer::createSocket()
{
    return new HttpSocket(this);
}

void HttpServer::handleRequest(HttpSocket * socket, const HttpRequest & request)
{
    socket->send(m_callback(request));
}

bool HttpServer::handleUpgrade(HttpSocket * socket, const HttpRequest & request)
{
    if (request.getHeader(http::Upgrade).getValue() == "websocket")
    {
        WebSocket * websocket = new WebSocket(this);

        websocket->upgrade(socket, request);

        return true;
    }

    return HttpSocketHandler::handleUpgrade(socket, request);
}

void HttpServer::handleText(WebSocket * socket, const QByteArray & text)
{
    socket->sendText(text);

    socket->sendPing();
}

void HttpServer::handleBinary(WebSocket * socket, const QByteArray & binary)
{
    socket->sendBinary(binary);
}

void HttpServer::connectionEstablished(WebSocket * socket)
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

} // namespace tastefulserver
