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

#include <tastefulserver/httpUtil.h>

#include <QString>
#include <QHash>
#include <QTextStream>

namespace tastefulserver {

class TASTEFULSERVER_API Cookie
{
public:
    static const QString MaxAge;
    static const QString Comment;
    static const QString Domain;
    static const QString Path;
    static const QString Version;
    static const QString Expires;
    static const QString Secure;
    static const QString HttpOnly;

public:
    Cookie();
    Cookie(const QString & name, const QString & value);

    QString getName() const;
    QString getValue() const;

    Cookie&setValue(const QString & value);
    Cookie&setMaxAge(unsigned seconds);
    Cookie&setComment(const QString & comment);
    Cookie&setDomain(const QString & domain);
    Cookie&setPath(const QString & path);
    Cookie&setVersion(const QString & version);
    Cookie&setExpires(const QDateTime & dateTime);
    Cookie&setExpires(const QString & expires);
    Cookie&setSecure(bool secure);
    Cookie&setHttpOnly(bool httpOnly);

    QString toString() const;

private:
    QString name;
    QString value;
    bool secure;
    bool httpOnly;
    QHash<QString, QString> attributes;
};

class TASTEFULSERVER_API Cookies
{
public:
    bool isEmpty() const;

    void set(const Cookie & cookie);
    bool has(const QString & name) const;
    Cookie get(const QString & name) const;
    Cookie &operator[](const QString & name);
    const Cookie operator[](const QString & name) const;
    void clear(Cookie & cookie);

    QHash<QString, Cookie>::iterator begin();
    QHash<QString, Cookie>::const_iterator begin() const;
    QHash<QString, Cookie>::iterator end();
    QHash<QString, Cookie>::const_iterator end() const;

    void parse(const QString & cookiesString);

    static Cookies fromString(const QString & cookiesString);

private:
    QHash<QString, Cookie> cookies;
};

} // namespace tastefulserver
