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

#include <tastefulserver/WebsocketHandler.h>

#include <QStringList>

#include <tastefulserver/HttpHeader.h>

namespace tastefulserver {

WebsocketHandler::WebsocketHandler()
{
}

void WebsocketHandler::receive(const QByteArray & data)
{
    int offset = 0;
    qint64 length = 0;

    char type = data[offset];
    ++offset;

    int opcode = type & 15;

//    *  %x0 denotes a continuation frame
//    *  %x1 denotes a text frame
//    *  %x2 denotes a binary frame
//    *  %x3-7 are reserved for further non-control frames
//    *  %x8 denotes a connection close
//    *  %x9 denotes a ping
//    *  %xA denotes a pong
//    *  %xB-F are reserved for further control frames

    bool final = (type & (1<<7)) != 0;

    bool reserved1 = (type & (1<<6)) != 0;
    bool reserved2 = (type & (1<<5)) != 0;
    bool reserved3 = (type & (1<<4)) != 0;

    qDebug() << "[" << final << reserved1 << reserved2 << reserved3 << "]" << opcode;


    char length_ = data[offset];
    ++offset;

    bool masked = (length_ & ~(1<<7)) != length_;

    length_ = length_ & ~(1<<7);

    qDebug() << (int)length_ << masked;


    length = length_;

    if (length_ == 126)
    {
        // next 2 bytes

        QByteArray l = data.mid(offset, 2);
        offset += 2;

        length = *reinterpret_cast<qint16*>(l.data());

    }
    else if (length_ == 127)
    {
        // next 8 bytes

        QByteArray l = data.mid(offset, 8);
        offset += 8;

        length = *reinterpret_cast<qint64*>(l.data());

    }

    QByteArray mask = data.mid(offset, 4);
    offset += 4;

    QByteArray d2(length, 0);

    int index = 0;

    for (int i = offset; i < data.length(); ++i)
    {
        d2[index] = (data[i] ^ mask[index % 4]);
        ++index;
    }


    qDebug() << length << "[" << (d2) << "]";

}

} // namespace tastefulserver
