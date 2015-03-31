#pragma once

#include <QSslCertificate>
#include <QSslKey>

#include <tastefulserver/tasteful-server_api.h>

#include <tastefulserver/TcpServer.h>
#include <tastefulserver/HttpSocketHandler.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API HttpsServer : public TcpServer, public HttpSocketHandler
{
public:
    HttpsServer(const QSslCertificate & certificate, const QSslKey & privateKey);
    ~HttpsServer();
private:
    QSslCertificate m_certificate;
    QSslKey m_privateKey;

    virtual AbstractSocket * createSocket() override;
};

} // namespace tastefulserver
