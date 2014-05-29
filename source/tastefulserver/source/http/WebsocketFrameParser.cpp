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
#include <cassert>

namespace tastefulserver {

const unsigned char WebsocketFrameParser::Length2Bytes = 126;
const unsigned char WebsocketFrameParser::Length4Bytes = 127;

WebsocketFrameParser::WebsocketFrameParser()
: m_state(ParseState::Header)
, m_interruptedState(ParseState::Header)
{
}

void WebsocketFrameParser::addData(const QByteArray & data)
{
    //m_byteStream.flush();
    m_byteStream.append(data);

    parse();
}

bool WebsocketFrameParser::hasReadyFrames() const
{
    return !m_readyFrames.empty();
}

WebsocketFrame WebsocketFrameParser::popReadyFrame()
{
    WebsocketFrame frame = m_readyFrames.back();
    m_readyFrames.pop_back();
    return frame;
}

void WebsocketFrameParser::pushFrame()
{
    m_readyFrames.push_back(m_currentFrame);
}

void WebsocketFrameParser::parse()
{
    while (true)
    {
        ParseState nextState = dispatch(m_state);

        if (nextState == ParseState::Interrupted)
        {
            m_interruptedState = m_state;

            break;
        }

        m_state = nextState;
    }
}

WebsocketFrameParser::ParseState WebsocketFrameParser::dispatch(ParseState state)
{
    switch (state)
    {
        case ParseState::Header:
            return parseFrameHeader();
        case ParseState::LengthMask:
            return parseLengthMask();
        case ParseState::ExtendedLength:
            return parseExtendedLength();
        case ParseState::Mask:
            return parseMask();
        case ParseState::Content:
            return parseContent();
        case ParseState::Finish:
            return finishFrame();
        case ParseState::Error:
            return handleError();
        default:
            return ParseState::Error;
    }
}

WebsocketFrameParser::ParseState WebsocketFrameParser::parseFrameHeader()
{
    if (m_byteStream.atEnd())
        return ParseState::Interrupted;

    length = 0;
    WebsocketFrame::Header header;
    header.raw = m_byteStream.readByte();

    m_currentFrame = WebsocketFrame(header);

    return ParseState::LengthMask;
}

WebsocketFrameParser::ParseState WebsocketFrameParser::parseLengthMask()
{
    if (m_byteStream.atEnd())
        return ParseState::Interrupted;

    lengthMask.raw = m_byteStream.readByte();

    if (lengthMask.data.len < Length2Bytes)
    {
        length = lengthMask.data.len;

        return ParseState::Mask;
    }
    else
    {
        return ParseState::ExtendedLength;
    }
}

WebsocketFrameParser::ParseState WebsocketFrameParser::parseExtendedLength()
{
    if (lengthMask.data.len == Length2Bytes)
    {
        if (m_byteStream.availableBytes() < 2)
        {
            return ParseState::Interrupted;
        }

        qint16 length2Bytes = *reinterpret_cast<qint16*>(m_byteStream.read(2).data());

        length = qFromBigEndian(length2Bytes);
    }
    else if (lengthMask.data.len == Length4Bytes)
    {
        if (m_byteStream.availableBytes() < 4)
        {
            return ParseState::Interrupted;
        }

        qint64 length4Bytes = *reinterpret_cast<qint64*>(m_byteStream.read(4).data());

        length = qFromBigEndian(length4Bytes);
    }
    else
    {
        return ParseState::Error;
    }

    return ParseState::Mask;
}

WebsocketFrameParser::ParseState WebsocketFrameParser::parseMask()
{
    if (lengthMask.data.mask == 0)
    {
        mask = { 0, 0, 0, 0 };
        m_currentFrame.setMask(mask);

        return ParseState::Content;
    }

    if (m_byteStream.availableBytes() < 4)
    {
        return ParseState::Interrupted;
    }

    QByteArray raw_mask = m_byteStream.read(4);

    mask = *reinterpret_cast<decltype(mask)*>(raw_mask.data());
    m_currentFrame.setMask(mask);

    return ParseState::Content;
}

WebsocketFrameParser::ParseState WebsocketFrameParser::parseContent()
{
    if (m_byteStream.availableBytes() < length)
    {
        return ParseState::Interrupted;
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

    return ParseState::Finish;
}

WebsocketFrameParser::ParseState WebsocketFrameParser::finishFrame()
{
    pushFrame();

    assert(m_byteStream.alreadyRead() == m_currentFrame.toByteArray());

    m_byteStream.flush();

    return ParseState::Header;
}

WebsocketFrameParser::ParseState WebsocketFrameParser::handleError()
{
    m_currentFrame.markBad();

    pushFrame();

    return ParseState::Header;
}

} // namespace tastefulserver
