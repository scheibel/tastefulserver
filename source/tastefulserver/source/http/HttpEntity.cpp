#include <tastefulserver/HttpEntity.h>

#include <QTextStream>

#include <QDataStream>
#include <QIODevice>

namespace tastefulserver {

HttpEntity::HttpEntity()
{
}

HttpEntity::~HttpEntity()
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

    return list.last();
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
            list.last().merge(header);
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

void HttpEntity::clearHeaders()
{
    m_headers.clear();
}

QByteArray HttpEntity::getContent() const
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

void HttpEntity::writeHeaderOn(const HttpHeader & header, QIODevice & device) const
{
    if (header.isValid())
    {
        device.write((header.toString() + http::Linebreak).toLocal8Bit());
    }
}

void HttpEntity::writeHeadersOn(QIODevice & device) const
{
    for (const QList<HttpHeader> & list : m_headers)
    {
        for (const HttpHeader & header : list)
        {
            writeHeaderOn(header, device);
        }
    }
}

void HttpEntity::writeTo(QIODevice & device) const
{
    writeHeadersOn(device);

    device.write(http::Linebreak.toLocal8Bit());

    if (!m_content.isNull())
    {
        device.write(m_content);
    }
}

} // namespace tastefulserver
