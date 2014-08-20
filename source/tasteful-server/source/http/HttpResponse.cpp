#include <tasteful-server/HttpResponse.h>

#include <QIODevice>
#include <QDateTime>

#include <tasteful-server/httpUtil.h>

namespace tastefulserver {

HttpResponse::HttpResponse(unsigned statusCode)
: m_statusCode(statusCode)
{
}

HttpResponse::HttpResponse(unsigned statusCode, const HttpVersion & httpVersion)
: HttpMessage(httpVersion)
, m_statusCode(statusCode)
{
}

HttpResponse::HttpResponse(unsigned statusCode, const HttpRequest & httpRequest)
: HttpMessage(httpRequest.getHttpVersion())
, m_statusCode(statusCode)
{
    if ((httpRequest.getHttpVersion() >= HttpVersion(1, 1)) || httpRequest.isKeepAlive())
    {
        keepAlive();
    }
}

void HttpResponse::setDate()
{
    setDate(QDateTime::currentDateTime());
}

void HttpResponse::setDate(const QDateTime & date)
{
    setHeader(http::Date, http::dateString(date));
}

void HttpResponse::setStatusCode(unsigned statusCode)
{
    m_statusCode = statusCode;
}

unsigned HttpResponse::getStatusCode() const
{
    return m_statusCode;
}

void HttpResponse::setCookie(const Cookie & cookie)
{
    m_cookies.set(cookie);
}

Cookie &HttpResponse::setCookie(const QString & key, const QString & value)
{
    setCookie(Cookie(key, value));

    return m_cookies[key];
}

void HttpResponse::writeHeadersOn(QIODevice & device) const
{
    HttpMessage::writeHeadersOn(device);

    for (const Cookie & cookie : m_cookies)
    {
        HttpHeader header(http::SetCookie, cookie.toString());
        writeHeaderOn(header, device);
    }
    HttpHeader header(http::ContentType, m_contentType.toString());
    writeHeaderOn(header, device);
}

void HttpResponse::writeTo(QIODevice & device) const
{
    device.write((m_httpVersion.toString() + " " + QString::number(m_statusCode)).toLocal8Bit());

    QString reason = http::reason(m_statusCode);
    if (!reason.isNull())
    {
        device.write((" " + reason).toLocal8Bit());
    }

    device.write(http::Linebreak.toLocal8Bit());

    HttpMessage::writeTo(device);
}

} // namespace tastefulserver
