#pragma once

#include <functional>

#include <QSslCertificate>
#include <QSslKey>

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/HttpsServer.h>

namespace tastefulserver {

class HttpRequest;
class HttpResponse;
class HttpSocket;

class TASTEFUL_SERVER_API HttpsCallbackServer : public HttpsServer
{
public:
    typedef std::function<HttpResponse(const HttpRequest &)> RequestCallback;

    HttpsCallbackServer(const QSslCertificate & certificate, const QSslKey & privateKey, const RequestCallback & callback);
    ~HttpsCallbackServer();
private:
    RequestCallback m_callback;

    // --- handling ---

    virtual void handleRequest(HttpSocket * socket, const HttpRequest & request) override;
};

} // namespace tastefulserver
