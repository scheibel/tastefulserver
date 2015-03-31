#pragma once

#include <tastefulserver/tasteful-server_api.h>

#include <tastefulserver/HttpSocket.h>

#include <QSslCertificate>
#include <QSslKey>

namespace tastefulserver {


class TASTEFUL_SERVER_API HttpsSocket : public HttpSocket
{
public:
    HttpsSocket(HttpSocketHandler * handler, const QSslCertificate & certificate, const QSslKey & privateKe);

protected:
    QSslCertificate m_certificate;
    QSslKey m_privateKey;

    virtual QAbstractSocket * createSocket(qintptr socketDescriptor) override;

    virtual void addConnectionInfo(HttpRequest & request) override;
};

} // namespace tastefulserver
