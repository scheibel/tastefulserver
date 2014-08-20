#pragma once

#include <functional>

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/TcpServer.h>
#include <tastefulserver/AbstractSocket.h>
#include <tastefulserver/HttpSocketHandler.h>
#include <tastefulserver/WebSocketHandler.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpServer : public TcpServer, public HttpSocketHandler, public WebSocketHandler
{
public:
    typedef std::function<HttpResponse(const HttpRequest &)> RequestCallback;

    HttpServer(const RequestCallback & callback);
    ~HttpServer();

protected:
    RequestCallback m_callback;

    virtual AbstractSocket * createSocket() override;

    // --- handling ---

    virtual void handleRequest(HttpSocket * socket, const HttpRequest & request) override;
    virtual bool handleUpgrade(HttpSocket * socket, const HttpRequest & request) override;

    virtual void handleText(WebSocket * socket, const QByteArray & text) override;
    virtual void handleBinary(WebSocket * socket, const QByteArray & binary) override;
    virtual void connectionEstablished(WebSocket * socket) override;
};

} // namespace tastefulserver
