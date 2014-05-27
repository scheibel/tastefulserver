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

#include <tastefulserver/WebsocketFrame.h>


namespace tastefulserver {

WebsocketFrame WebsocketFrame::fromByteArray(const QByteArray & data)
{
    qint64 length = 0;

    Header header;
    header.raw = data[0];

    union {
        struct {
#ifdef LITTLE_ENDIAN
            unsigned int len : 7;
            unsigned int mask : 1;
#else
            unsigned int mask : 1;
            unsigned int len : 7;
#endif
        } data;
        char raw;
    } lengthIndicator;

    const static unsigned char Next2Bytes = 126;
    const static unsigned char Next4Bytes = 127;

    lengthIndicator.raw = data[1];

    int offset = 2;

    if (lengthIndicator.data.len == Next2Bytes)
    {
        offset = 4;
        length = *reinterpret_cast<qint16*>(data.mid(2, 2).data());
    }
    else if (lengthIndicator.data.len == Next4Bytes)
    {
        offset = 6;
        length = *reinterpret_cast<qint64*>(data.mid(2, 4).data());
    }

    QByteArray mask(4, 0);

    if (lengthIndicator.data.mask == 1)
    {
        mask = data.mid(offset, 4);
        offset += 4;
    }

    QByteArray content(length, 0);

    for (int i = offset, index = 0; i < data.length(); ++i, ++index)
    {
        content[index] = (data[i] ^ mask[index % 4]);
    }

    WebsocketFrame frame(header);
    frame.setContent(content);

    return frame;
}

WebsocketFrame::WebsocketFrame()
{
    m_header.raw = 0;
}

WebsocketFrame::WebsocketFrame(const Header & header)
: m_header(header)
{
}

void WebsocketFrame::setHeader(const Header & header)
{
    m_header = header;
}

const WebsocketFrame::Header & WebsocketFrame::getHeader() const
{
    return m_header;
}

void WebsocketFrame::setContent(const QByteArray & content)
{
    m_content = content;
}

const QByteArray & WebsocketFrame::getContent() const
{
    return m_content;
}

WebsocketFrame::OpCode WebsocketFrame::getOpCode() const
{
    return static_cast<OpCode>(m_header.data.opcode);
}

} // namespace tastefulserver
