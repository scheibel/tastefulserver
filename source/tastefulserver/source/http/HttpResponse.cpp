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

void HttpResponse::writeHeadersOn(QByteArray & byteArray) const
{
    HttpMessage::writeHeadersOn(byteArray);

    for (const Cookie & cookie : m_cookies)
    {
        HttpHeader header(http::SetCookie, cookie.toString());
        writeHeaderOn(header, byteArray);
    }
    HttpHeader header(http::ContentType, m_contentType.toString());
    writeHeaderOn(header, byteArray);
}

QByteArray HttpResponse::toByteArray() const
{
    QByteArray byteArray;

    byteArray.append(m_httpVersion.toString() + " " + QString::number(m_statusCode));
    QString reason = http::reason(m_statusCode);
    if (!reason.isNull())
    {
        byteArray.append(" " + reason);
    }
    byteArray.append(http::Linebreak);

    byteArray.append(HttpMessage::toByteArray());

    return byteArray;
}

} // namespace tastefulserver
