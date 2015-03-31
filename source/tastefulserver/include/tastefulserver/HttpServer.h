#pragma once

#include <tastefulserver/tasteful-server_api.h>

#include <tastefulserver/TcpServer.h>
#include <tastefulserver/HttpSocketHandler.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API HttpServer : public TcpServer, public HttpSocketHandler
{
public:
    HttpServer();
    ~HttpServer();

protected:
    virtual AbstractSocket * createSocket() override;

    // --- handling ---

    //virtual void handleRequest(HttpSocket * socket, const HttpRequest & request) override;
};

} // namespace tastefulserver
