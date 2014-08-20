#include <tasteful-server/WebSocketServer.h>

#include <tasteful-server/WebSocket.h>

namespace tastefulserver {

WebSocketServer::WebSocketServer()
{
}

WebSocketServer::~WebSocketServer()
{
}

AbstractSocket * WebSocketServer::createSocket()
{
    return new WebSocket(this);
}

} // namespace tastefulserver
