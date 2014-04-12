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

#include <QString>
#include <QByteArray>

namespace tastefulserver {

class TASTEFULSERVER_API ByteArrayStream
{
public:
    ByteArrayStream(const QByteArray & bytes, const QString & linebreak = "\n");

    static ByteArrayStream forLinebreak(const QString & linebreak);

    void append(const QByteArray & bytes);
    QByteArray read(int length);
    void skip(int length);
    QString readLine();
    void skipUpTo(const QString & delimiter);
    void skipUpTo(const QChar & delimiter);
    void skipBehind(const QString & delimiter);
    void skipBehind(const QChar & delimiter);
    bool canReadUpTo(const QString & delimiter);
    bool canReadUpTo(const QChar & delimiter);
    QByteArray readUpTo(const QString & delimiter, bool skipDelimiter = false);
    QByteArray readUpTo(const QChar & delimiter, bool skipDelimiter = false);
    QByteArray readAll();
    void skipAll();
    bool canReadLine();
    int availableBytes();
    void flush();
    bool atEnd();

protected:
    QByteArray buffer;
    QString linebreak;
    int pos;
};

} // namespace tastefulserver
