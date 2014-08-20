#include <tastefulserver/HttpsServer.h>

#include <tastefulserver/HttpsSocket.h>

namespace tastefulserver {

HttpsServer::HttpsServer(const QSslCertificate & certificate, const QSslKey & privateKey)
: TcpServer()
, m_certificate(certificate)
, m_privateKey(privateKey)
{
}

HttpsServer::~HttpsServer()
{
}

AbstractSocket * HttpsServer::createSocket()
{
    return new HttpsSocket(this, m_certificate, m_privateKey);
}

} // namespace tastefulserver
