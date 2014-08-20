#include <tasteful-server/HttpWebSocketServer.h>

#include <tasteful-server/HttpSocket.h>
#include <tasteful-server/WebSocket.h>

#include <QTimer>

namespace tastefulserver {

HttpWebSocketServer::HttpWebSocketServer()
{
}

HttpWebSocketServer::~HttpWebSocketServer()
{
}

AbstractSocket * HttpWebSocketServer::createSocket()
{
    return new HttpSocket(this);
}

bool HttpWebSocketServer::handleUpgrade(HttpSocket * socket, const HttpRequest & request)
{
    if (request.getHeader(http::Upgrade).getValue() == "websocket")
    {
        WebSocket * websocket = new WebSocket(this);

        websocket->upgrade(socket, request);

        return true;
    }

    return HttpSocketHandler::handleUpgrade(socket, request);
}

} // namespace tastefulserver
