#pragma once

#include <QByteArray>
#include <QHash>

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpHeader.h>

class QTextStream;
class QDataStream;
template <typename T>
class QList;
class QString;

namespace tastefulserver {

namespace http {
const QString Linebreak = "\r\n";
}

class TASTEFULSERVER_API HttpEntity
{
public:
    HttpEntity();

    bool hasHeader(const http::HeaderName & headerName) const;
    HttpHeader getHeader(const http::HeaderName & headerName) const;
    QList<HttpHeader> getHeaders(const http::HeaderName & headerName) const;

    void addHeader(const HttpHeader & header);
    void addHeaders(const QList<HttpHeader> & headers);

    void setHeader(const HttpHeader & header, bool merge = false);
    void setHeader(const http::HeaderName & headerName, const QString & value, bool merge = false);

    void removeHeader(const http::HeaderName & headerName);

    void setContent(const QByteArray & content);
    const QByteArray & getContent() const;
    void clearContent();

    virtual QByteArray toByteArray() const;

protected:
    QHash<QString, QList<HttpHeader >> m_headers;
    QByteArray m_content;

    void writeHeaderOn(const HttpHeader & header, QByteArray & byteArray) const;
    virtual void writeHeadersOn(QByteArray & stream) const;
};

} // namespace tastefulserver
