#include <tastefulserver/HttpsSocket.h>

#include <QSslSocket>

namespace tastefulserver {

HttpsSocket::HttpsSocket(HttpSocketHandler * handler, const QSslCertificate & certificate, const QSslKey & privateKey)
: HttpSocket(handler)
, m_certificate(certificate)
, m_privateKey(privateKey)
{
}


QAbstractSocket * HttpsSocket::createSocket(qintptr socketDescriptor)
{
    QSslSocket * socket = new QSslSocket();

    socket->setSocketDescriptor(socketDescriptor);

    socket->setLocalCertificate(m_certificate);
    socket->setPrivateKey(m_privateKey);

    socket->startServerEncryption();

    return socket;
}

void HttpsSocket::addConnectionInfo(HttpRequest & request)
{
    HttpSocket::addConnectionInfo(request);
    request.setHttps(true);
}


} // namespace tastefulserver
