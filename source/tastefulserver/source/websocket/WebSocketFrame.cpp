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

#include <tastefulserver/WebSocketFrame.h>

#include <QDataStream>
#include <QtEndian>

#include <limits>
#include <cstring>

namespace tastefulserver {

const unsigned char WebSocketFrame::Length2Bytes = 126;
const unsigned char WebSocketFrame::Length4Bytes = 127;

WebSocketFrame::WebSocketFrame()
: m_masked(false)
{
    m_header.raw = 0;
}

WebSocketFrame::WebSocketFrame(const Header & header)
: m_header(header)
, m_masked(false)
{
}

WebSocketFrame::WebSocketFrame(OpCode opCode, bool isFinal)
: m_masked(false)
{
    m_header.raw = 0;
    m_header.data.opcode = (unsigned int)opCode;
    m_header.data.fin = isFinal ? 1 : 0;
}

WebSocketFrame::WebSocketFrame(OpCode opCode, const QByteArray & content)
: WebSocketFrame(opCode, true)
{
    setContent(content);
    setRandomMask();
}

void WebSocketFrame::setHeader(const Header & header)
{
    m_header = header;
}

const WebSocketFrame::Header & WebSocketFrame::getHeader() const
{
    return m_header;
}

void WebSocketFrame::setContent(const QByteArray & content)
{
    m_content = content;
}

const QByteArray & WebSocketFrame::getContent() const
{
    return m_content;
}

void WebSocketFrame::append(const QByteArray & data)
{
    m_content.append(data);
}

void WebSocketFrame::setMask(const std::array<char, 4> & mask)
{
    m_masked = true;

    m_mask = mask;
}

void WebSocketFrame::setMask(int mask)
{
    m_masked = true;

    m_mask = *reinterpret_cast<decltype(m_mask)*>(&mask);
}

void WebSocketFrame::setRandomMask()
{
    setMask(qrand());
}

WebSocketFrame::OpCode WebSocketFrame::getOpCode() const
{
    return static_cast<OpCode>(m_header.data.opcode);
}

bool WebSocketFrame::isControlFrame() const
{
    return getOpCode() >= OpCode::ConnectionClose;
}

bool WebSocketFrame::isFinal() const
{
    return m_header.data.fin == 1;
}

bool WebSocketFrame::isContinuationFrame() const
{
    return getOpCode() == OpCode::Continuation;
}

QByteArray WebSocketFrame::toByteArray() const
{
    int headerLength = 2;
    qint64 contentLength = m_content.length();

    LengthMask lengthMask;
    lengthMask.raw = 0;

    if (contentLength < Length2Bytes)
    {
        lengthMask.data.len = contentLength;
    }
    else if (contentLength < std::numeric_limits<qint16>::max())
    {
        lengthMask.data.len = Length2Bytes;
        headerLength += 2;
    }
    else
    {
        lengthMask.data.len = Length4Bytes;
        headerLength += 4;
    }

    if (m_masked)
    {
        lengthMask.data.mask = 1;
        headerLength += 4;
    }

    qint64 totalLength = headerLength + contentLength;

    // write data

    QByteArray byteArray(totalLength, 0);

    byteArray[0] = m_header.raw;
    byteArray[1] = lengthMask.raw;

    if (lengthMask.data.len == Length2Bytes)
    {
        qint16 len = qToBigEndian(static_cast<qint16>(contentLength));
        memcpy(&byteArray.data()[2], &len, sizeof((len)));
    }
    else if (lengthMask.data.len == Length4Bytes)
    {
        qint64 len = qToBigEndian(static_cast<qint64>(contentLength));
        memcpy(&byteArray.data()[2], &len, sizeof(len));
    }

    if (m_masked)
    {
        memcpy(&byteArray.data()[headerLength-4], &m_mask, 4);
    }

    for (int i = 0; i < contentLength; ++i)
    {
        byteArray[headerLength + i] = (m_content[i] ^ m_mask[i % 4]);
    }

    return byteArray;
}

} // namespace tastefulserver
