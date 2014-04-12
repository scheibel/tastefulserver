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

#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpHeader.h>

#include <QString>
#include <QByteArray>
#include <QList>
#include <QHash>
#include <QTextStream>

namespace tastefulserver {

namespace http {
const QString Linebreak = "\r\n";
}

class TASTEFULSERVER_API HttpEntity
{
public:
    HttpEntity();

    bool hasHeader(const http::HeaderName & headerName) const;
    HttpHeader getHeader(const http::HeaderName & headerName) const;
    QList<HttpHeader> getHeaders(const http::HeaderName & headerName) const;

    void addHeader(const HttpHeader & header);
    void addHeaders(const QList<HttpHeader> & headers);

    void setHeader(const HttpHeader & header, bool merge = false);
    void setHeader(const http::HeaderName & headerName, const QString & value, bool merge = false);

    void removeHeader(const http::HeaderName & headerName);

    void setContent(const QByteArray & content);
    QByteArray getContent() const;
    void clearContent();

    virtual QByteArray toByteArray() const;

protected:
    QHash<QString, QList<HttpHeader >> headers;
    QByteArray content;

    void writeHeaderOn(const HttpHeader & header, QTextStream & stream) const;
    virtual void writeHeadersOn(QTextStream & stream) const;
};

} // namespace tastefulserver
