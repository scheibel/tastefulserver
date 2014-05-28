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

#include <tastefulserver/WebsocketFrameParser.h>

#include <QtEndian>
#include <QDebug>

namespace tastefulserver {

const unsigned char WebsocketFrameParser::Next2Bytes = 126;
const unsigned char WebsocketFrameParser::Next4Bytes = 127;

WebsocketFrameParser::WebsocketFrameParser()
: m_state(State::ParseFrameHeader)
{
}

void WebsocketFrameParser::addData(const QByteArray & data)
{
    m_byteStream.append(data);
    parse();
}

void WebsocketFrameParser::parse()
{
    bool continueReading = true;

    while (continueReading)
    {
        State nextState = State::Wait;

        switch (m_state)
        {
            case State::Wait:
                break;
            case State::ParseFrameHeader:
                nextState = parseFrameHeader();
                break;
            case State::ParseLengthMask:
                nextState = parseLengthMask();
                break;
            case State::ParseExtendedLength:
                nextState = parseExtendedLength();
                break;
            case State::ParseMask:
                nextState = parseMask();
                break;
            case State::ParseContent:
                nextState = parseContent();
                break;
            case State::FinishFrame:
                nextState = finishFrame();
                break;
            case State::HandleError:
                nextState = handleError();
                break;
        }

        if (nextState == State::Wait)
            break;

        m_state = nextState;
    }
}

WebsocketFrameParser::State WebsocketFrameParser::parseFrameHeader()
{
    if (m_byteStream.atEnd())
        return State::Wait;

    length = 0;
    WebsocketFrame::Header header;
    header.raw = m_byteStream.readByte();

    m_currentFrame.setHeader(header);

    qDebug() << "header = " << (int)(unsigned char)header.raw;
    qDebug() << "opcode = " << (int)header.data.opcode;

    return State::ParseLengthMask;
}

WebsocketFrameParser::State WebsocketFrameParser::parseLengthMask()
{
    if (m_byteStream.atEnd())
        return State::Wait;

    lengthMask.raw = m_byteStream.readByte();

    if (lengthMask.data.len < Next2Bytes)
    {
        length = lengthMask.data.len;

        qDebug() << "Length = " << length;

        return State::ParseMask;
    }
    else
    {
        return State::ParseExtendedLength;
    }
}

WebsocketFrameParser::State WebsocketFrameParser::parseExtendedLength()
{
    if (lengthMask.data.len == Next2Bytes)
    {
        if (m_byteStream.availableBytes() < 2)
            return State::Wait;

        qint16 length2Bytes = *reinterpret_cast<qint16*>(m_byteStream.read(2).data());

        length = qFromBigEndian(length2Bytes);

        qDebug() << "Extended 2 Length = " << length;
    }
    else if (lengthMask.data.len == Next4Bytes)
    {
        if (m_byteStream.availableBytes() < 4)
            return State::Wait;

        qint64 length4Bytes = *reinterpret_cast<qint64*>(m_byteStream.read(4).data());

        length = qFromBigEndian(length4Bytes);

        qDebug() << "Extended 4 Length = " << length;
    }
    else
    {
        return State::HandleError;
    }

    return State::ParseMask;
}

WebsocketFrameParser::State WebsocketFrameParser::parseMask()
{
    if (lengthMask.data.mask == 0)
    {
        mask = { 0, 0, 0, 0 };

        return State::ParseContent;
    }

    if (m_byteStream.availableBytes() < 4)
        return State::Wait;

    mask = *reinterpret_cast<decltype(mask)*>(m_byteStream.read(4).data());

    qDebug() << "mask: " << (int)(unsigned char)mask[0] << (int)(unsigned char)mask[1] << (int)(unsigned char)mask[2] << (int)(unsigned char)mask[3];

    return State::ParseContent;
}

WebsocketFrameParser::State WebsocketFrameParser::parseContent()
{
    if (m_byteStream.availableBytes() < length)
    {
        return State::Wait;
    }

    QByteArray content = m_byteStream.read(length);

    if (lengthMask.data.mask == 1)
    {
        for (int i = 0; i < content.length(); ++i)
        {
            content[i] = content[i] ^ mask[i % 4];
        }
    }

    m_currentFrame.setContent(content);

    return State::FinishFrame;
}

WebsocketFrameParser::State WebsocketFrameParser::finishFrame()
{
    if (m_currentFrame.getOpCode() == WebsocketFrame::OpCode::Text)
    {
        qDebug() << "content: " << m_currentFrame.getContent();
    }
    else if (m_currentFrame.getOpCode() == WebsocketFrame::OpCode::Binary)
    {
        QString s;
        for (char c : m_currentFrame.getContent())
        {
            s += QString::number((int)c);
        }
        qDebug() << "content:"<<s;
    }
    else
    {
    }

    qDebug() << "----";

    return State::ParseFrameHeader;
}

WebsocketFrameParser::State WebsocketFrameParser::handleError()
{
    qDebug() << "error";

    return State::Wait;
}

} // namespace tastefulserver
