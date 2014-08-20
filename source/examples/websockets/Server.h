#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpWebSocketServer.h>

class TASTEFULSERVER_API Server : public tastefulserver::HttpWebSocketServer
{
public:
    Server();
    ~Server();

protected:
    virtual void handleRequest(tastefulserver::HttpSocket * socket, const tastefulserver::HttpRequest & request) override;

    virtual void handleText(tastefulserver::WebSocket * socket, const QByteArray & text) override;
    virtual void handleBinary(tastefulserver::WebSocket * socket, const QByteArray & binary) override;
    virtual void connectionEstablished(tastefulserver::WebSocket * socket) override;
};
