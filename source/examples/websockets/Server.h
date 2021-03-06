#pragma once

#include <tastefulserver/HttpWebSocketServer.h>

class Server : public tastefulserver::HttpWebSocketServer
{
public:
    Server();
    virtual ~Server();

protected:
    virtual void handleRequest(tastefulserver::HttpSocket * socket, const tastefulserver::HttpRequest & request) override;

    virtual void handleText(tastefulserver::WebSocket * socket, const QByteArray & text) override;
    virtual void handleBinary(tastefulserver::WebSocket * socket, const QByteArray & binary) override;
    virtual void connectionEstablished(const tastefulserver::HttpRequest & request, tastefulserver::WebSocket * socket) override;
};
