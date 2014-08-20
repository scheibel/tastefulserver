#include <tastefulserver/WebSocketServer.h>

#include <tastefulserver/WebSocket.h>

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
