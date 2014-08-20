#include <tasteful-server/HttpMessage.h>

#include <QTextStream>

namespace tastefulserver {

HttpMessage::HttpMessage()
    : m_httpVersion(HttpVersion(1, 1))
{
}

HttpMessage::HttpMessage(const HttpVersion & httpVersion)
    : m_httpVersion(httpVersion)
{
}

HttpVersion &HttpMessage::getHttpVersion()
{
    return m_httpVersion;
}

const HttpVersion &HttpMessage::getHttpVersion() const
{
    return m_httpVersion;
}

Cookies &HttpMessage::getCookies()
{
    return m_cookies;
}

const Cookies &HttpMessage::getCookies() const
{
    return m_cookies;
}

bool HttpMessage::hasCookies() const
{
    return !m_cookies.isEmpty();
}

ContentType &HttpMessage::getContentType()
{
    return m_contentType;
}

const ContentType &HttpMessage::getContentType() const
{
    return m_contentType;
}

void HttpMessage::keepAlive()
{
    setHeader(http::Connection, "Keep-Alive");
}

bool HttpMessage::isKeepAlive() const
{
    return getHeader(http::Connection).getValue().toLower()=="keep-alive";
}

int HttpMessage::getContentLength() const
{
    return getHeader(http::ContentLength).getValue().toInt();
}

bool HttpMessage::isMultiPart() const
{
    return m_contentType.isMultiPart();
}

MultiPart &HttpMessage::getMultiPart()
{
    return m_multiPart;
}

const MultiPart &HttpMessage::getMultiPart() const
{
    return m_multiPart;
}

} // namespace tastefulserver
