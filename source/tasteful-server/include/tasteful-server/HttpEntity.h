#pragma once

#include <QByteArray>
#include <QHash>

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/HttpHeader.h>

class QIODevice;
template <typename T>
class QList;
class QString;

namespace tastefulserver {

namespace http {
const QString Linebreak = "\r\n";
}

class TASTEFUL_SERVER_API HttpEntity
{
public:
    HttpEntity();
    virtual ~HttpEntity();

    bool hasHeader(const http::HeaderName & headerName) const;
    HttpHeader getHeader(const http::HeaderName & headerName) const;
    QList<HttpHeader> getHeaders(const http::HeaderName & headerName) const;

    void addHeader(const HttpHeader & header);
    void addHeaders(const QList<HttpHeader> & headers);

    void setHeader(const HttpHeader & header, bool merge = false);
    void setHeader(const http::HeaderName & headerName, const QString & value, bool merge = false);

    void removeHeader(const http::HeaderName & headerName);

    void clearHeaders();

    void setContent(const QByteArray & content);
    const QByteArray & getContent() const;
    void clearContent();

    virtual void writeTo(QIODevice & device) const;
protected:
    QHash<QString, QList<HttpHeader>> m_headers;
    QByteArray m_content;

    void writeHeaderOn(const HttpHeader & header, QIODevice & device) const;
    virtual void writeHeadersOn(QIODevice & device) const;
};

} // namespace tastefulserver
