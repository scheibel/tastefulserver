#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/TcpServer.h>
#include <tastefulserver/HttpSocketHandler.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpServer : public TcpServer, public HttpSocketHandler
{
public:
    HttpServer(int numThreads = 1);
    ~HttpServer();

protected:
    virtual AbstractSocket * createSocket() override;

    // --- handling ---

    //virtual void handleRequest(HttpSocket * socket, const HttpRequest & request) override;
};

} // namespace tastefulserver
