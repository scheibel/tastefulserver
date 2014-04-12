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

#include <tastefulserver/MultiPart.h>
#include <tastefulserver/HttpEntity.h>
#include <tastefulserver/HttpHeader.h>
#include <tastefulserver/ByteArrayStream.h>

namespace tastefulserver {

Part::Part()
{
}

Part::Part(const QList<HttpHeader> & headers, const QByteArray & content)
    : content(content)
{
    for (const HttpHeader & header : headers)
    {
        this->headers.insert(header.getName(), header);
    }
}

bool Part::hasHeader(const http::HeaderName & headerName) const
{
    return headers.contains(headerName);
}

HttpHeader Part::getHeader(const http::HeaderName & headerName) const
{
    return headers[headerName];
}

QList<HttpHeader> Part::getHeaders() const
{
    return headers.values();
}

QByteArray Part::getContent() const
{
    return content;
}

MultiPart::MultiPart()
{
}

MultiPart::MultiPart(const ContentType & type)
    : type(type)
{
}

bool MultiPart::isFormData() const
{
    return type.getSubtype()==ContentType::FormData;
}

QList<Part> MultiPart::getParts() const
{
    return parts;
}

void MultiPart::parse(const QByteArray & content)
{
    if (!type.isMultiPart())
    {
        return;
    }

    QString boundary = type.getBoundary();
    if (boundary.isEmpty())
    {
        return;
    }
    boundary = "--" + boundary;
    ByteArrayStream stream(content, http::Linebreak);

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
        parts << Part(headers, partContent);
    }
}

} // namespace tastefulserver
