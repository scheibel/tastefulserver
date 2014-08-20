#pragma once

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/WebSocket.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API WebSocketHandler
{
public:
    virtual void connectionEstablished(WebSocket * socket);

    virtual void handleText(WebSocket * socket, const QByteArray & text) = 0;
    virtual void handleBinary(WebSocket * socket, const QByteArray & binary) = 0;
};

} // namespace tastefulserver
