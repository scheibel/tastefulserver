#pragma once

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/TcpServer.h>
#include <tasteful-server/HttpSocketHandler.h>

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
