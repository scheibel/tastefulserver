/**
 * (C) LGPL-3
 *
 * Tasteful Server <https://github.com/scheibel/tasteful-server>
 *
 * Copyright: 2012 Lux, Scheibel
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

#include <tastefulserver/HttpEntity.h>

#include <QTextStream>

namespace tastefulserver {

HttpEntity::HttpEntity()
{
}

bool HttpEntity::hasHeader(const http::HeaderName & headerName) const
{
    return headers.contains(headerName);
}

HttpHeader HttpEntity::getHeader(const http::HeaderName & headerName) const
{
    const QList<HttpHeader> & list = headers[headerName];

    if (list.isEmpty())
    {
        return HttpHeader::forName(headerName);
    }

    return list.first();
}

QList<HttpHeader> HttpEntity::getHeaders(const http::HeaderName & headerName) const
{
    return headers[headerName];
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
    headers[header.getName()] << header;
}

void HttpEntity::setHeader(const http::HeaderName & headerName, const QString & value, bool merge)
{
    setHeader(HttpHeader(headerName, value), merge);
}

void HttpEntity::setHeader(const HttpHeader & header, bool merge)
{
    QList<HttpHeader> & list = headers[header.getName()];
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
    headers.remove(headerName);
}

QByteArray HttpEntity::getContent() const
{
    return content;
}

void HttpEntity::clearContent()
{
    content.clear();
    removeHeader(http::ContentLength);
}

void HttpEntity::setContent(const QByteArray & content)
{
    this->content = content;
    setHeader(http::ContentLength, QString::number(content.size()));
}

void HttpEntity::writeHeaderOn(const HttpHeader & header, QTextStream & stream) const
{
    if (header.isValid())
    {
        stream << header.toString() << http::Linebreak;
    }
}

void HttpEntity::writeHeadersOn(QTextStream & stream) const
{
    for (const QList<HttpHeader> & list : headers)
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
    QTextStream stream(&byteArray);

    writeHeadersOn(stream);

    stream << http::Linebreak;

    if (!content.isNull())
    {
        stream << content;
    }

    return byteArray;
}

} // namespace tastefulserver
