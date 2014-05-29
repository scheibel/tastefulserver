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

#include <tastefulserver/Protocol.h>
#include <tastefulserver/WebsocketFrame.h>
#include <tastefulserver/WebsocketFrameParser.h>
#include <tastefulserver/http.h>

namespace tastefulserver {

class WebsocketHandler;

class TASTEFULSERVER_API WebsocketProtocol : public Protocol
{
    Q_OBJECT
public:
    WebsocketProtocol(WebsocketHandler * handler);

    void handshake(const HttpRequest & request);

    void sendText(const QByteArray & text);
    void sendBinary(const QByteArray & binary);
    void sendPing();

    void closeConnection();
protected:
    WebsocketHandler * m_handler;
    WebsocketFrameParser m_parser;
    WebsocketFrame m_fragmentedMessage;
    bool m_inFragmentedMode;

    virtual void receiveData(const QByteArray & data) override;

    void sendFrame(const WebsocketFrame & frame);
    void sendPong();

    static const QString MagicString;
    static QString hashKey(const QString & key);
protected slots:
    void badFrame();
};

} // namespace tastefulserver
