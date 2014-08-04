#include <tastefulserver/HttpsServer.h>

#include "../server/SocketCreation.h"

namespace tastefulserver {

HttpsServer::HttpsServer(const QSslCertificate & certificate, const QSslKey & privateKey, const HttpHandler::RequestCallback & callback)
    : HttpServer(callback)
    , m_certificate(certificate)
    , m_privateKey(privateKey)
{
}

ConnectionHandler * HttpsServer::createConnectionHandler(qintptr socketDescriptor) const
{
    HttpHandler * https = new HttpHandler(m_callback);

    https->setSocketCreator(new SslSocketCreation(socketDescriptor, m_certificate, m_privateKey));

    return https;
}

} // namespace tastefulserver
