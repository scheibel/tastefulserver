#pragma once

#include <functional>

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpServer.h>
#include <tastefulserver/AbstractSocket.h>
#include <tastefulserver/HttpSocketHandler.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpCallbackServer : public HttpServer
{
public:
    typedef std::function<HttpResponse(const HttpRequest &)> RequestCallback;

    HttpCallbackServer(const RequestCallback & callback, int numThreads = 1);
    ~HttpCallbackServer();

protected:
    RequestCallback m_callback;

    // --- handling ---

    virtual void handleRequest(HttpSocket * socket, const HttpRequest & request) override;
};

} // namespace tastefulserver
