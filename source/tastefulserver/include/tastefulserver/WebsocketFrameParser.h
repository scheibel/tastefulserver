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

#include <tastefulserver/ByteStream.h>
#include <tastefulserver/WebsocketFrame.h>

#include <QQueue>
#include <QByteArray>
#include <array>

namespace tastefulserver {

class TASTEFULSERVER_API WebsocketFrameParser
{
public:
    WebsocketFrameParser();

    void addData(const QByteArray & data);

    bool hasReadyFrames() const;
    WebsocketFrame popReadyFrame();
protected:
    enum class ParseState
    {
        Header,
        LengthMask,
        ExtendedLength,
        Mask,
        Content,
        Finish,
        Error,
        Interrupted
    };

    ByteStream m_byteStream;
    WebsocketFrame m_currentFrame;
    ParseState m_state;
    ParseState m_interruptedState;
    QQueue<WebsocketFrame> m_readyFrames;

    void parse();

    ParseState dispatch(ParseState state);

    ParseState parseFrameHeader();
    ParseState parseLengthMask();
    ParseState parseExtendedLength();
    ParseState parseMask();
    ParseState parseContent();
    ParseState finishFrame();
    ParseState handleError();

    void pushFrame();

    qint64 length;
    WebsocketFrame::LengthMask lengthMask;
    std::array<char, 4> mask;

    static const unsigned char Length2Bytes;
    static const unsigned char Length4Bytes;
};

} // namespace tastefulserver
