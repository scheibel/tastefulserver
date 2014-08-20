#include <tastefulserver/HttpsCallbackServer.h>

namespace tastefulserver {

HttpsCallbackServer::HttpsCallbackServer(const QSslCertificate & certificate, const QSslKey & privateKey, const RequestCallback & callback)
: HttpsServer(certificate, privateKey)
, m_callback(callback)
{
}

HttpsCallbackServer::~HttpsCallbackServer()
{
}

void HttpsCallbackServer::handleRequest(HttpSocket * socket, const HttpRequest & request)
{
    socket->send(m_callback(request));
}

} // namespace tastefulserver
