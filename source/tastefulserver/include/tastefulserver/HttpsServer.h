#pragma once

#include <QSslCertificate>
#include <QSslKey>

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpServer.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpsServer : public HttpServer
{
public:
    HttpsServer(const QSslCertificate & certificate, const QSslKey & privateKey, const RequestCallback & callback);
    ~HttpsServer();
private:
    QSslCertificate m_certificate;
    QSslKey m_privateKey;

    virtual AbstractSocket * createSocket() override;
};

} // namespace tastefulserver
