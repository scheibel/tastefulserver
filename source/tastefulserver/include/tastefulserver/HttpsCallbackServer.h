#pragma once

#include <functional>

#include <QSslCertificate>
#include <QSslKey>

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpsServer.h>

namespace tastefulserver {

class HttpRequest;
class HttpResponse;
class HttpSocket;

class TASTEFULSERVER_API HttpsCallbackServer : public HttpsServer
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
