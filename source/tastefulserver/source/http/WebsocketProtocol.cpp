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

#include <tastefulserver/WebsocketProtocol.h>

#include <QCryptographicHash>

#include <tastefulserver/WebsocketHandler.h>


namespace tastefulserver {

const QString WebsocketProtocol::MagicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

QString WebsocketProtocol::hashKey(const QString & key)
{
    return QCryptographicHash::hash((key + MagicString).toUtf8(), QCryptographicHash::Sha1).toBase64();
}

HttpResponse WebsocketProtocol::handshake(const HttpRequest & request)
{
    HttpResponse response(http::SwitchingProtocols);

    response.setHeader(http::Upgrade, "websocket");
    response.setHeader(http::Connection, http::Upgrade);
    response.setHeader(http::SecWebSocketAccept, hashKey(request.getHeader(http::SecWebSocketKey).getValue()));

    //response.addHeader(request.getHeader(http::SecWebSocketProtocol));

    return response;
}

WebsocketProtocol::WebsocketProtocol(WebsocketHandler * handler)
: m_handler(handler)
{
    connect(&m_parser, &WebsocketFrameParser::badFrame, this, &WebsocketProtocol::badFrame);
}

void WebsocketProtocol::receiveData(const QByteArray & data)
{
    m_parser.addData(data);

    while (m_parser.hasReadyFrames())
    {
        m_handler->handleFrame(this, m_parser.popReadyFrame());
    }
}

void WebsocketProtocol::badFrame()
{
    m_handler->handleBadFrame(this);
}

void WebsocketProtocol::send(const WebsocketFrame & frame)
{
    sendData(frame.toByteArray());
}

void WebsocketProtocol::sendText(const QString & text)
{
    send(WebsocketFrame(WebsocketFrame::OpCode::Text, text.toLatin1()));
}

void WebsocketProtocol::sendBinary(const QByteArray & binary)
{
    send(WebsocketFrame(WebsocketFrame::OpCode::Binary, binary));
}

void WebsocketProtocol::sendPing()
{
    send(WebsocketFrame(WebsocketFrame::OpCode::Ping));
}

void WebsocketProtocol::sendPong()
{
    send(WebsocketFrame(WebsocketFrame::OpCode::Pong));
}

void WebsocketProtocol::sendConnectionClose()
{
    send(WebsocketFrame(WebsocketFrame::OpCode::ConnectionClose));
}


} // namespace tastefulserver
