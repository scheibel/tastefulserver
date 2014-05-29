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

#include <QDataStream>
#include <QtEndian>

#include <limits>
#include <cstring>

namespace tastefulserver {

WebsocketFrame::WebsocketFrame()
{
    m_header.raw = 0;
}

WebsocketFrame::WebsocketFrame(const Header & header)
: m_header(header)
{
}

WebsocketFrame::WebsocketFrame(OpCode opCode, bool isFinal)
{
    m_header.raw = 0;
    m_header.data.opcode = (unsigned int)opCode;
    m_header.data.fin = isFinal ? 1 : 0;
}

WebsocketFrame::WebsocketFrame(OpCode opCode, const QByteArray & content)
: WebsocketFrame(opCode, true)
{
    setContent(content);
    setRandomMask();
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

void WebsocketFrame::setMask(const std::array<char, 4> & mask)
{
    m_mask = mask;
}

void WebsocketFrame::setMask(int mask)
{
    m_mask = *reinterpret_cast<decltype(m_mask)*>(&mask);
}

void WebsocketFrame::setRandomMask()
{
    setMask(qrand());
}

WebsocketFrame::OpCode WebsocketFrame::getOpCode() const
{
    return static_cast<OpCode>(m_header.data.opcode);
}

bool WebsocketFrame::isContinuation() const
{
    return getOpCode() == OpCode::Continuation;
}

bool WebsocketFrame::isText() const
{
    return getOpCode() == OpCode::Text;
}

bool WebsocketFrame::isBinary() const
{
    return getOpCode() == OpCode::Binary;
}

bool WebsocketFrame::isConnectionClose() const
{
    return getOpCode() == OpCode::ConnectionClose;
}

bool WebsocketFrame::isPing() const
{
    return getOpCode() == OpCode::Ping;
}

bool WebsocketFrame::isPong() const
{
    return getOpCode() == OpCode::Pong;
}

QByteArray WebsocketFrame::toByteArray() const
{
    LengthMask lengthMask;
    lengthMask.data.mask = (m_mask != std::array<char, 4>{{0,0,0,0}});

    qint64 length = m_content.length();

    int headerLength = 2;

    if (length < 126)
    {
        lengthMask.data.len = length;
    }
    else if (length < std::numeric_limits<qint16>::max())
    {
        lengthMask.data.len = 126;

        headerLength += 2;
    }
    else
    {
        lengthMask.data.len = 127;

        headerLength += 4;
    }

    QByteArray byteArray(headerLength+4+length, 0);

    byteArray[0] = m_header.raw;
    byteArray[1] = lengthMask.raw;

    if (lengthMask.data.len == 126)
    {
        qint16 len = qToBigEndian(static_cast<qint16>(length));

        memcpy(&byteArray.data()[2], &len, sizeof((len)));
    }
    else if (lengthMask.data.len == 127)
    {
        qint64 len = qToBigEndian(static_cast<qint64>(length));
        memcpy(&byteArray.data()[2], &len, sizeof(len));
    }

    int offset = headerLength;

    memcpy(&byteArray.data()[offset], &m_mask, 4);

    offset += 4;

    for (int i = 0; i < length; ++i)
    {
        byteArray[i+offset] = (m_content[i] ^ m_mask[i % 4]);
    }

    return byteArray;
}

} // namespace tastefulserver
