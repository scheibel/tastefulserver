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

#include <QTextStream>
#include <QtEndian>

#include <limits>

namespace tastefulserver {

WebsocketFrame::WebsocketFrame()
: m_bad(true)
{
    m_header.raw = 0;
}

WebsocketFrame::WebsocketFrame(const Header & header)
: m_bad(false)
, m_header(header)
{
}

WebsocketFrame::WebsocketFrame(OpCode opCode, bool isFinal)
: m_bad(false)
{
    m_header.raw = 0;
    m_header.data.opcode = (unsigned int)opCode;
    m_header.data.fin = isFinal ? 1 : 0;
}

bool WebsocketFrame::isBad() const
{
    return m_bad;
}

void WebsocketFrame::markBad()
{
    m_bad = true;
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
    QByteArray byteArray;
    QTextStream stream(&byteArray);

    stream << m_header.raw;

    LengthMask lengthMask;
    lengthMask.data.mask = 1;

    qint64 length = m_content.length();

    if (length < 126)
    {
        lengthMask.data.len = length;
        stream << lengthMask.raw;
    }
    else if (length < std::numeric_limits<qint16>::max())
    {
        lengthMask.data.len = 126;
        stream << lengthMask.raw << qToBigEndian(static_cast<qint16>(length));
    }
    else
    {
        lengthMask.data.len = 127;
        stream << lengthMask.raw << qToBigEndian(static_cast<qint64>(length));
    }

    stream << m_mask[0] << m_mask[1] << m_mask[2] << m_mask[3];

    for (int i = 0; i < length; ++i)
    {
        stream << (m_content[i] ^ m_mask[i % 4]);
    }

    return byteArray;
}

} // namespace tastefulserver
