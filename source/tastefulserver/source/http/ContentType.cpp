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

#include <tastefulserver/ContentType.h>

#include <QDebug>

namespace tastefulserver {

const QString ContentType::Charset = "charset";
const QString ContentType::Boundary = "boundary";
const QString ContentType::Multipart = "multipart";
const QString ContentType::Application = "application";
const QString ContentType::FormData = "form-data";
const QString ContentType::XWWWFormUrlEncoded = "x-www-form-urlencoded";
ContentType::ContentType()
{
}

ContentType::ContentType(const QString & type, const QString & subtype)
    : type(type)
    , subtype(subtype)
{
    updateName();
}

void ContentType::updateName()
{
    name = type + '/' + subtype;
}

bool ContentType::is(const QString & type, const QString & subtype) const
{
    return this->type==type && this->subtype==subtype;
}

bool ContentType::operator==(const ContentType & contentType) const
{
    return type == contentType.type && subtype == contentType.subtype;
}

void ContentType::setType(const QString & type)
{
    this->type = type;

    updateName();
}

void ContentType::setSubtype(const QString & subtype)
{
    this->subtype = subtype;

    updateName();
}

void ContentType::setTypeAndSubtype(const QString & typeAndSubtype)
{
    int pos = typeAndSubtype.indexOf('/');

    if (pos<0)
    {
        type = typeAndSubtype;
        subtype = QString();
    }
    else
    {
        type = typeAndSubtype.left(pos);
        subtype = typeAndSubtype.mid(pos + 1);
    }

    updateName();
}

ContentType ContentType::fromString(const QString & value)
{
    ContentType contentType;

    contentType.parse(value);

    return contentType;
}

void ContentType::parse(const QString & value)
{
    HttpHeaderElement::parse(value);

    setTypeAndSubtype(name);
}

const QString &ContentType::getType() const
{
    return type;
}

const QString &ContentType::getSubtype() const
{
    return subtype;
}

QString ContentType::getCharset() const
{
    return getParameter(Charset);
}

QString ContentType::getBoundary() const
{
    return getParameter(Boundary);
}

bool ContentType::isMultiPart() const
{
    return type==Multipart;
}

} // namespace tastefulserver
