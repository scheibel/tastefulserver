#include <tastefulserver/HttpEntity.h>

#include <QTextStream>
#include <QDataStream>

namespace tastefulserver {

HttpEntity::HttpEntity()
{
}

bool HttpEntity::hasHeader(const http::HeaderName & headerName) const
{
    return m_headers.contains(headerName);
}

HttpHeader HttpEntity::getHeader(const http::HeaderName & headerName) const
{
    const QList<HttpHeader> & list = m_headers[headerName];

    if (list.isEmpty())
    {
        return HttpHeader::forName(headerName);
    }

    return list.first();
}

QList<HttpHeader> HttpEntity::getHeaders(const http::HeaderName & headerName) const
{
    return m_headers[headerName];
}

void HttpEntity::addHeaders(const QList<HttpHeader> & headers)
{
    for (const HttpHeader & header : headers)
    {
        addHeader(header);
    }
}

void HttpEntity::addHeader(const HttpHeader & header)
{
    m_headers[header.getName()] << header;
}

void HttpEntity::setHeader(const http::HeaderName & headerName, const QString & value, bool merge)
{
    setHeader(HttpHeader(headerName, value), merge);
}

void HttpEntity::setHeader(const HttpHeader & header, bool merge)
{
    QList<HttpHeader> & list = m_headers[header.getName()];
    if (list.isEmpty())
    {
        list << header;
    }
    else
    {
        if (merge)
        {
            list.first().merge(header);
        }
        else
        {
            list[0] = header;
        }
    }
}

void HttpEntity::removeHeader(const http::HeaderName & headerName)
{
    m_headers.remove(headerName);
}

const QByteArray & HttpEntity::getContent() const
{
    return m_content;
}

void HttpEntity::clearContent()
{
    m_content.clear();
    removeHeader(http::ContentLength);
}

void HttpEntity::setContent(const QByteArray & content)
{
    m_content = content;
    setHeader(http::ContentLength, QString::number(content.size()));
}

void HttpEntity::writeHeaderOn(const HttpHeader & header, QByteArray & byteArray) const
{
    if (header.isValid())
    {
        byteArray.append(header.toString() + http::Linebreak);
    }
}

void HttpEntity::writeHeadersOn(QByteArray & stream) const
{
    for (const QList<HttpHeader> & list : m_headers)
    {
        for (const HttpHeader & header : list)
        {
            writeHeaderOn(header, stream);
        }
    }
}

QByteArray HttpEntity::toByteArray() const
{
    QByteArray byteArray;

    writeHeadersOn(byteArray);

    byteArray.append(http::Linebreak);

    if (!m_content.isNull())
    {
        byteArray.append(m_content);
    }

    return byteArray;
}

} // namespace tastefulserver
