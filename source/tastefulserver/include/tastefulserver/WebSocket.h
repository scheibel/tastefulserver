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

#include <tastefulserver/AbstractSocket.h>
#include <tastefulserver/WebSocketFrame.h>
#include <tastefulserver/WebSocketFrameParser.h>
#include <tastefulserver/http.h>

namespace tastefulserver {

class WebSocketHandler;

class TASTEFULSERVER_API WebSocket : public AbstractSocket
{
    Q_OBJECT
public:
    WebSocket(WebSocketHandler * handler);

    void handshake(const HttpRequest & request);

    void sendText(const QByteArray & text);
    void sendBinary(const QByteArray & binary);
    void sendPing();

    void closeConnection();
protected:
    WebSocketHandler * m_handler;
    WebSocketFrameParser m_parser;
    WebSocketFrame m_fragmentedMessage;
    bool m_inFragmentedMode;

    virtual void receiveData(const QByteArray & data) override;

    void sendFrame(const WebSocketFrame & frame);
    void sendPong();

    static const QString MagicString;
    static QString hashKey(const QString & key);
protected slots:
    void badFrame();
};

} // namespace tastefulserver
