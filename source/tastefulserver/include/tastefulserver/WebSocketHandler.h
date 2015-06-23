#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/WebSocket.h>

namespace tastefulserver {

class TASTEFULSERVER_API WebSocketHandler
{
public:
    virtual void connectionEstablished(const HttpRequest & request, WebSocket * socket);
    virtual void connectionClosed(WebSocket * socket);

    virtual void handleText(WebSocket * socket, const QByteArray & text) = 0;
    virtual void handleBinary(WebSocket * socket, const QByteArray & binary) = 0;
};

} // namespace tastefulserver
