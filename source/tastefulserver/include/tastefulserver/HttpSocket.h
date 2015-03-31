#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/AbstractSocket.h>
#include <tastefulserver/HttpRequestParser.h>
#include <tastefulserver/http.h>

namespace tastefulserver {

class HttpSocketHandler;

class TASTEFULSERVER_API HttpSocket : public AbstractSocket
{
public:
    HttpSocket(HttpSocketHandler * handler);

    void send(const HttpResponse & response);

    void disconnect();
protected:
    virtual void receiveData(const QByteArray & data) override;

    HttpSocketHandler * m_handler;
    HttpRequestParser m_parser;
    HttpRequest m_request;

    virtual void addConnectionInfo(HttpRequest & request);

    virtual QAbstractSocket * createSocket(qintptr socketDescriptor) override;

protected slots:
    void badRequest();
};

} // namespace tastefulserver
