#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpSocket.h>

#include <QSslCertificate>
#include <QSslKey>

namespace tastefulserver {


class TASTEFULSERVER_API HttpsSocket : public HttpSocket
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
