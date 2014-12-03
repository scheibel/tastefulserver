#pragma once

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/TcpServer.h>
#include <tasteful-server/HttpSocketHandler.h>
#include <tasteful-server/WebSocketHandler.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API HttpWebSocketServer : public TcpServer, public HttpSocketHandler, public WebSocketHandler
{
public:
    HttpWebSocketServer();
    virtual ~HttpWebSocketServer();

protected:
    virtual AbstractSocket * createSocket() override;

    // --- handling ---

    virtual bool handleUpgrade(HttpSocket * socket, const HttpRequest & request) override;
    //virtual void handleRequest(HttpSocket * socket, const HttpRequest & request) override;

    //virtual void handleText(WebSocket * socket, const QByteArray & text) override;
    //virtual void handleBinary(WebSocket * socket, const QByteArray & binary) override;
    //virtual void connectionEstablished(WebSocket * socket) override;
};

} // namespace tastefulserver
