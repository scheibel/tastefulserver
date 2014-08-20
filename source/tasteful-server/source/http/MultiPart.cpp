#include <tasteful-server/MultiPart.h>

#include <tasteful-server/HttpEntity.h>
#include <tasteful-server/HttpHeader.h>
#include <tasteful-server/ByteStream.h>

namespace tastefulserver {

Part::Part()
{
}

Part::Part(const QList<HttpHeader> & headers, const QByteArray & content)
    : m_content(content)
{
    for (const HttpHeader & header : headers)
    {
        m_headers.insert(header.getName(), header);
    }
}

bool Part::hasHeader(const http::HeaderName & headerName) const
{
    return m_headers.contains(headerName);
}

HttpHeader Part::getHeader(const http::HeaderName & headerName) const
{
    return m_headers[headerName];
}

QList<HttpHeader> Part::getHeaders() const
{
    return m_headers.values();
}

QByteArray Part::getContent() const
{
    return m_content;
}

MultiPart::MultiPart()
{
}

MultiPart::MultiPart(const ContentType & type)
    : m_type(type)
{
}

bool MultiPart::isFormData() const
{
    return m_type.getSubtype()==ContentType::FormData;
}

QList<Part> MultiPart::getParts() const
{
    return m_parts;
}

void MultiPart::parse(const QByteArray & content)
{
    if (!m_type.isMultiPart())
    {
        return;
    }

    QString boundary = m_type.getBoundary();
    if (boundary.isEmpty())
    {
        return;
    }
    boundary = "--" + boundary;
    ByteStream stream(content, http::Linebreak);

    stream.skipBehind(boundary);
    stream.skip(2);
    while (stream.canReadUpTo(boundary))
    {
        QList<HttpHeader> headers;
        while (stream.canReadLine())
        {
            QString line = stream.readLine();
            if (line.isEmpty())
            {
                break;
            }
            headers << HttpHeader::fromString(line);
        }
        QByteArray partContent = stream.readUpTo(http::Linebreak + boundary, true);
        stream.skip(2);
        m_parts << Part(headers, partContent);
    }
}

} // namespace tastefulserver
