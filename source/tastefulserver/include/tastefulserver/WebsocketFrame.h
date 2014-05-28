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

#include <QByteArray>

namespace tastefulserver {

class TASTEFULSERVER_API WebsocketFrame
{
public:
    union Header {
        struct {
            #ifdef LITTLE_ENDIAN
            unsigned int opcode : 4;
            unsigned int fin : 1;
            unsigned int reserved1 : 1;
            unsigned int reserved2 : 1;
            unsigned int reserved3 : 1;
            #else
            unsigned int fin : 1;
            unsigned int reserved1 : 1;
            unsigned int reserved2 : 1;
            unsigned int reserved3 : 1;
            unsigned int opcode : 4;
            #endif
        } data;
        char raw;
    };

    enum class OpCode : unsigned char {
        Continuation = 0x0,
        Text = 0x1,
        Binary = 0x2,
        // 0x3-0x7 reserved for further non-control frames
        ConnectionClose = 0x8,
        Ping = 0x9,
        Pong = 0xA
        // 0xB-0xF are reserved for further control frames
    };

    static WebsocketFrame fromByteArray(const QByteArray & data);

    WebsocketFrame();
    WebsocketFrame(const Header & header);

    void setHeader(const Header & header);
    const Header & getHeader() const;

    void setContent(const QByteArray & content);
    const QByteArray & getContent() const;

    OpCode getOpCode() const;
protected:
    Header m_header;
    QByteArray m_content;
};

} // namespace tastefulserver