#include <tastefulserver/HttpResponse.h>

#include <QDateTime>

#include <tastefulserver/httpUtil.h>

namespace tastefulserver {

HttpResponse::HttpResponse()
{
    initialize();
}

HttpResponse::HttpResponse(const HttpVersion & httpVersion)
    : HttpMessage(httpVersion)
{
    initialize();
}

HttpResponse::HttpResponse(const HttpRequest & httpRequest)
    : HttpMessage(httpRequest.getHttpVersion())
{
    initialize();
    if ((httpRequest.getHttpVersion()>=HttpVersion(1, 1)) || httpRequest.isKeepAlive())
    {
        keepAlive();
    }
}

void HttpResponse::initialize()
{
    setStatusCode(http::NotFound);
    setHeader(http::ContentLength, "0");
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
