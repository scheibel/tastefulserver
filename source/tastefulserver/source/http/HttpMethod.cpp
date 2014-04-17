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

#include <tastefulserver/HttpMethod.h>

#include <QString>
#include <QHash>

namespace tastefulserver {

HttpMethod::HttpMethod()
    : m_method(INVALID)
{
}

HttpMethod::HttpMethod(const Method & method)
    : m_method(method)
{
}

HttpMethod::HttpMethod(const HttpMethod & method)
    : m_method(method.m_method)
{
}

HttpMethod HttpMethod::fromString(const QString & methodString)
{
    static QHash<QString, Method> hash = makeHash();

    return HttpMethod(hash.value(methodString.toUpper(), INVALID));
}

QHash<QString, HttpMethod::Method> HttpMethod::makeHash()
{
    QHash<QString, Method> hash;

    hash.insert("GET", GET);
    hash.insert("POST", POST);
    hash.insert("PUT", PUT);
    hash.insert("DELETE", DELETE);
    hash.insert("UPDATE", UPDATE);
    hash.insert("TRACE", TRACE);
    hash.insert("HEAD", HEAD);
    hash.insert("OPTIONS", OPTIONS);
    hash.insert("CONNECT", CONNECT);

    return hash;
}

HttpMethod &HttpMethod::operator=(const Method & method)
{
    m_method = method;

    return *this;
}

HttpMethod &HttpMethod::operator=(const HttpMethod & method)
{
    m_method = method.m_method;

    return *this;
}

bool HttpMethod::operator==(const Method & method) const
{
    return m_method == method;
}

bool HttpMethod::operator==(const HttpMethod & method) const
{
    return m_method == method.m_method;
}

bool HttpMethod::operator!=(const Method & method) const
{
    return m_method != method;
}

bool HttpMethod::operator!=(const HttpMethod & method) const
{
    return m_method != method.m_method;
}

HttpMethod::operator unsigned() const
{
    return m_method;
}

HttpMethod::operator Method() const
{
    return m_method;
}

bool HttpMethod::isInvalid() const
{
    return m_method == INVALID;
}

HttpMethod::Method HttpMethod::method() const
{
    return m_method;
}

QString HttpMethod::toString() const
{
    switch (m_method)
    {
        case GET:

            return "GET";
        case POST:

            return "POST";
        case PUT:

            return "PUT";
        case DELETE:

            return "DELETE";
        case UPDATE:

            return "UPDATE";
        case TRACE:

            return "TRACE";
        case HEAD:

            return "HEAD";
        case OPTIONS:

            return "OPTIONS";
        case CONNECT:

            return "CONNECT";
        default:

            return "INVALID";
    }
}

} // namespace tastefulserver
