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

#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpHeader.h>

#include <QString>

namespace tastefulserver {

class TASTEFULSERVER_API ContentType : public HttpHeaderElement
{
public:
    static const QString Charset;
    static const QString Boundary;
    static const QString Multipart;
    static const QString Application;
    static const QString FormData;
    static const QString XWWWFormUrlEncoded;

    ContentType();
    ContentType(const QString & type, const QString & subtype);
    static ContentType fromString(const QString & value);

    bool is(const QString & type, const QString & subtype) const;
    bool operator==(const ContentType & contentType) const;

    void setType(const QString & type);
    void setSubtype(const QString & subtype);
    void setTypeAndSubtype(const QString & typeAndSubype);

    const QString & getType() const;
    const QString & getSubtype() const;

    QString getCharset() const;
    QString getBoundary() const;

    bool isMultiPart() const;

    void parse(const QString & value);

protected:
    void updateName();

    QString m_type;
    QString m_subtype;
};

} // namespace tastefulserver
