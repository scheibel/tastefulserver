/**
 * (C) LGPL-3
 *
 * Tasteful Server <https://github.com/scheibel/tasteful-server>
 *
 * Copyright: 2012-2014 Lux, Scheibel
 * Authors:
 *     Roland Lux <rollux2000@googlemail.com>
 *     Willy Scheibel <willyscheibel@gmx.de>
 *
 * This file is part of Tasteful Server.
 *
 * Tasteful Server is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tasteful Server is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Tasteful Server.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <tastefulserver/HttpResponse.h>

#include <QTextStream>
#include <QDateTime>

#include <tastefulserver/httpUtil.h>

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

void HttpResponse::writeHeadersOn(QTextStream & stream) const
{
    HttpMessage::writeHeadersOn(stream);

    for (const Cookie & cookie : m_cookies)
    {
        HttpHeader header(http::SetCookie, cookie.toString());
        writeHeaderOn(header, stream);
    }
    HttpHeader header(http::ContentType, m_contentType.toString());
    writeHeaderOn(header, stream);
}

QByteArray HttpResponse::toByteArray() const
{
    QByteArray byteArray;
    QTextStream stream(&byteArray);

    stream << m_httpVersion.toString() << " " << m_statusCode;
    QString reason = http::reason(m_statusCode);
    if (!reason.isNull())
    {
        stream << " " << reason;
    }
    stream << http::Linebreak;

    stream << HttpMessage::toByteArray();

    return byteArray;
}

} // namespace tastefulserver
