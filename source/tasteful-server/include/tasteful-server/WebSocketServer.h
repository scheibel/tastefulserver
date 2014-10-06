#pragma once

#include <functional>

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/TcpServer.h>
#include <tasteful-server/AbstractSocket.h>
#include <tasteful-server/WebSocketHandler.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API WebSocketServer : public TcpServer, public WebSocketHandler
{
public:
    WebSocketServer();
    ~WebSocketServer();

protected:
    virtual AbstractSocket * createSocket() override;

    // --- handling ---

    //virtual void handleText(WebSocket * socket, const QByteArray & text) override;
    //virtual void handleBinary(WebSocket * socket, const QByteArray & binary) override;
    //virtual void connectionEstablished(WebSocket * socket) override;
    //virtual void connectionClosed(WebSocket * socket) override;
};

} // namespace tastefulserver
