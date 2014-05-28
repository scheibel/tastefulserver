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

const unsigned char WebsocketFrameParser::Length2Bytes = 126;
const unsigned char WebsocketFrameParser::Length4Bytes = 127;

WebsocketFrameParser::WebsocketFrameParser()
: m_state(State::ParseFrameHeader)
, m_haltedState(State::ParseFrameHeader)
{
}

void WebsocketFrameParser::addData(const QByteArray & data)
{
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

void WebsocketFrameParser::parse()
{
    while (!m_byteStream.atEnd())
    {
        m_state = dispatch(m_state);

        if (m_state == State::ContinueLater)
        {
            m_state = m_haltedState;

            break;
        }
    }
}

WebsocketFrameParser::State WebsocketFrameParser::dispatch(State state)
{
    switch (state)
    {
        case State::ParseFrameHeader:
            return parseFrameHeader();
        case State::ParseLengthMask:
            return parseLengthMask();
        case State::ParseExtendedLength:
            return parseExtendedLength();
        case State::ParseMask:
            return parseMask();
        case State::ParseContent:
            return parseContent();
        case State::FinishFrame:
            return finishFrame();
        case State::HandleError:
            return handleError();
        default:
            return State::ContinueLater;
    }
}

WebsocketFrameParser::State WebsocketFrameParser::continueLater()
{
    m_haltedState = m_state;
    return State::ContinueLater;
}

WebsocketFrameParser::State WebsocketFrameParser::parseFrameHeader()
{
    if (m_byteStream.atEnd())
        return continueLater();

    length = 0;
    WebsocketFrame::Header header;
    header.raw = m_byteStream.readByte();

    m_currentFrame = WebsocketFrame(header);

    return State::ParseLengthMask;
}

WebsocketFrameParser::State WebsocketFrameParser::parseLengthMask()
{
    if (m_byteStream.atEnd())
        return continueLater();

    lengthMask.raw = m_byteStream.readByte();

    if (lengthMask.data.len < Length2Bytes)
    {
        length = lengthMask.data.len;

        return State::ParseMask;
    }
    else
    {
        return State::ParseExtendedLength;
    }
}

WebsocketFrameParser::State WebsocketFrameParser::parseExtendedLength()
{
    if (lengthMask.data.len == Length2Bytes)
    {
        if (m_byteStream.availableBytes() < 2)
        {
            return continueLater();
        }

        qint16 length2Bytes = *reinterpret_cast<qint16*>(m_byteStream.read(2).data());

        length = qFromBigEndian(length2Bytes);
    }
    else if (lengthMask.data.len == Length4Bytes)
    {
        if (m_byteStream.availableBytes() < 4)
        {
            return continueLater();
        }

        qint64 length4Bytes = *reinterpret_cast<qint64*>(m_byteStream.read(4).data());

        length = qFromBigEndian(length4Bytes);
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
        m_currentFrame.setMask(mask);

        return State::ParseContent;
    }

    if (m_byteStream.availableBytes() < 4)
    {
        return continueLater();
    }

    mask = *reinterpret_cast<decltype(mask)*>(m_byteStream.read(4).data());
    m_currentFrame.setMask(mask);

    return State::ParseContent;
}

WebsocketFrameParser::State WebsocketFrameParser::parseContent()
{
    if (m_byteStream.availableBytes() < length)
    {
        return continueLater();
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
    m_readyFrames.push_back(m_currentFrame);

    return State::ParseFrameHeader;
}

WebsocketFrameParser::State WebsocketFrameParser::handleError()
{
    m_currentFrame.markBad();
    m_readyFrames.push_back(m_currentFrame);

    return State::ParseFrameHeader;
}

} // namespace tastefulserver
