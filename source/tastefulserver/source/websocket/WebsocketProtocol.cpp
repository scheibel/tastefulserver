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


WebsocketProtocol::WebsocketProtocol(WebsocketHandler * handler)
: m_handler(handler)
, m_inFragmentedMode(false)
{
    connect(&m_parser, &WebsocketFrameParser::badFrame, this, &WebsocketProtocol::badFrame);
}

QString WebsocketProtocol::hashKey(const QString & key)
{
    return QCryptographicHash::hash((key + MagicString).toUtf8(), QCryptographicHash::Sha1).toBase64();
}

void WebsocketProtocol::handshake(const HttpRequest & request)
{
    HttpResponse response(http::SwitchingProtocols);

    response.setHeader(http::Upgrade, "websocket");
    response.setHeader(http::Connection, http::Upgrade);
    response.setHeader(http::SecWebSocketAccept, hashKey(request.getHeader(http::SecWebSocketKey).getValue()));

    //response.addHeader(request.getHeader(http::SecWebSocketProtocol)); // sub protocols

    sendData(response.toByteArray());

    m_handler->connectionEstablished(this);
}

void WebsocketProtocol::receiveData(const QByteArray & data)
{
    m_parser.addData(data);

    while (m_parser.hasReadyFrames())
    {
        WebsocketFrame frame = m_parser.popReadyFrame();

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
            case WebsocketFrame::OpCode::Text:
                m_handler->handleText(this, frame.getContent());
                break;
            case WebsocketFrame::OpCode::Binary:
                m_handler->handleBinary(this, frame.getContent());
                break;
            case WebsocketFrame::OpCode::Ping:
                sendPong();
                break;
            case WebsocketFrame::OpCode::Pong:
                qDebug() << "received pong";
                break;
            case WebsocketFrame::OpCode::ConnectionClose:
                sendFrame(WebsocketFrame(WebsocketFrame::OpCode::ConnectionClose));
                disconnect();
                break;
            default:
                break;
        }
    }
}

void WebsocketProtocol::badFrame()
{
    disconnect();
}

void WebsocketProtocol::sendFrame(const WebsocketFrame & frame)
{
    sendData(frame.toByteArray());
}

void WebsocketProtocol::sendText(const QByteArray & text)
{
    sendFrame(WebsocketFrame(WebsocketFrame::OpCode::Text, text));
}

void WebsocketProtocol::sendBinary(const QByteArray & binary)
{
    sendFrame(WebsocketFrame(WebsocketFrame::OpCode::Binary, binary));
}

void WebsocketProtocol::sendPing()
{
    sendFrame(WebsocketFrame(WebsocketFrame::OpCode::Ping));
}

void WebsocketProtocol::sendPong()
{
    sendFrame(WebsocketFrame(WebsocketFrame::OpCode::Pong));
}

void WebsocketProtocol::closeConnection()
{
    sendFrame(WebsocketFrame(WebsocketFrame::OpCode::ConnectionClose));
    disconnect();
}


} // namespace tastefulserver
