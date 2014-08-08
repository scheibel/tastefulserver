#pragma once

#include <QUrl>
#include <QHostAddress>

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpMethod.h>
#include <tastefulserver/HttpMessage.h>
#include <tastefulserver/RequestParameters.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpRequest : public HttpMessage
{
public:
    HttpRequest();
    HttpRequest(const HttpMethod & method, const QString & requestUri, const HttpVersion & httpVersion, bool isHttps = false);

    bool isBad() const;
    void markBad();

    bool isXMLHttpRequest() const;

    void parseHeader(const HttpHeader & header);
    void parseContent(const QByteArray & content);

    const QHostAddress & address() const;
    void setAddress(const QHostAddress & address);

    unsigned port() const;
    void setPort(unsigned port);

    HttpMethod getMethod() const;
    QUrl getUrl() const;
    QString getPath() const;
    QString getRequestUri() const;

    RequestParameters&getParameters();
    const RequestParameters&getParameters() const;
    virtual void writeTo(QIODevice & device) const;

protected:
    QHostAddress m_address;
    unsigned m_port;
    bool m_bad;
    HttpMethod m_method;
    QString m_requestUri;
    QUrl m_url;
    RequestParameters m_requestParams;
};

} // namespace tastefulserver
