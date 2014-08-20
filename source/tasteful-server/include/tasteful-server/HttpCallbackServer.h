#pragma once

#include <functional>

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/HttpServer.h>
#include <tasteful-server/AbstractSocket.h>
#include <tasteful-server/HttpSocketHandler.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API HttpCallbackServer : public HttpServer
{
public:
    typedef std::function<HttpResponse(const HttpRequest &)> RequestCallback;

    HttpCallbackServer(const RequestCallback & callback);
    ~HttpCallbackServer();

protected:
    RequestCallback m_callback;

    // --- handling ---

    virtual void handleRequest(HttpSocket * socket, const HttpRequest & request) override;
};

} // namespace tastefulserver
