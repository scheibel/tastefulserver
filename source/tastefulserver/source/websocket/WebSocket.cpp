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

#include <tastefulserver/WebSocket.h>

#include <QCryptographicHash>

#include <tastefulserver/WebSocketHandler.h>


namespace tastefulserver {

const QString WebSocket::MagicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";


WebSocket::WebSocket(WebSocketHandler * handler)
: m_handler(handler)
, m_inFragmentedMode(false)
{
    connect(&m_parser, &WebSocketFrameParser::badFrame, this, &WebSocket::badFrame);
}

QString WebSocket::hashKey(const QString & key)
{
    return QCryptographicHash::hash((key + MagicString).toUtf8(), QCryptographicHash::Sha1).toBase64();
}

void WebSocket::handshake(const HttpRequest & request)
{
    HttpResponse response(http::SwitchingProtocols);

    response.setHeader(http::Upgrade, "websocket");
    response.setHeader(http::Connection, http::Upgrade);
    response.setHeader(http::SecWebSocketAccept, hashKey(request.getHeader(http::SecWebSocketKey).getValue()));

    //response.addHeader(request.getHeader(http::SecWebSocket)); // sub protocols

    sendData(response.toByteArray());

    m_handler->connectionEstablished(this);
}

void WebSocket::receiveData(const QByteArray & data)
{
    m_parser.addData(data);

    while (m_parser.hasReadyFrames())
    {
        WebSocketFrame frame = m_parser.popReadyFrame();

        if (!frame.isFinal())
        {
            if (frame.isControlFrame())
            {
                disconnect();
                break;
            }

            if (!m_inFragmentedMode)
            {
                m_inFragmentedMode = true;
                m_fragmentedMessage = frame;
            }
            else
            {
                if (!frame.isContinuationFrame())
                {
                    disconnect();
                    break;
                }

                m_fragmentedMessage.append(frame.getContent());
            }

            continue;
        }

        if (frame.isContinuationFrame())
        {
            if (!m_inFragmentedMode)
            {
                disconnect();
                break;
            }

            m_fragmentedMessage.append(frame.getContent());
            m_inFragmentedMode = false;

            frame = m_fragmentedMessage;
        }

        switch (frame.getOpCode())
        {
            case WebSocketFrame::OpCode::Text:
                m_handler->handleText(this, frame.getContent());
                break;
            case WebSocketFrame::OpCode::Binary:
                m_handler->handleBinary(this, frame.getContent());
                break;
            case WebSocketFrame::OpCode::Ping:
                sendPong();
                break;
            case WebSocketFrame::OpCode::Pong:
                qDebug() << "received pong";
                break;
            case WebSocketFrame::OpCode::ConnectionClose:
                sendFrame(WebSocketFrame(WebSocketFrame::OpCode::ConnectionClose));
                disconnect();
                break;
            default:
                break;
        }
    }
}

void WebSocket::badFrame()
{
    disconnect();
}

void WebSocket::sendFrame(const WebSocketFrame & frame)
{
    sendData(frame.toByteArray());
}

void WebSocket::sendText(const QByteArray & text)
{
    sendFrame(WebSocketFrame(WebSocketFrame::OpCode::Text, text));
}

void WebSocket::sendBinary(const QByteArray & binary)
{
    sendFrame(WebSocketFrame(WebSocketFrame::OpCode::Binary, binary));
}

void WebSocket::sendPing()
{
    sendFrame(WebSocketFrame(WebSocketFrame::OpCode::Ping));
}

void WebSocket::sendPong()
{
    sendFrame(WebSocketFrame(WebSocketFrame::OpCode::Pong));
}

void WebSocket::closeConnection()
{
    sendFrame(WebSocketFrame(WebSocketFrame::OpCode::ConnectionClose));
    disconnect();
}


} // namespace tastefulserver
