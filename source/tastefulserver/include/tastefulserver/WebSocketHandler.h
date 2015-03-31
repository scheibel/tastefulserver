#pragma once

#include <tastefulserver/tasteful-server_api.h>

#include <tastefulserver/WebSocket.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API WebSocketHandler
{
public:
    virtual void connectionEstablished(WebSocket * socket);
    virtual void connectionClosed(WebSocket * socket);

    virtual void handleText(WebSocket * socket, const QByteArray & text) = 0;
    virtual void handleBinary(WebSocket * socket, const QByteArray & binary) = 0;
};

} // namespace tastefulserver
