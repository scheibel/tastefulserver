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

#include <tastefulserver/HttpServer.h>
#include <tastefulserver/WebsocketProtocol.h>

#include <QTimer>

namespace tastefulserver {

HttpServer::HttpServer(const RequestCallback & callback)
: m_callback(callback)
, m_socketFactory(new TcpSocketFactory)
{
}

HttpServer::~HttpServer()
{
    delete m_socketFactory;
}

SocketFactory * HttpServer::getSocketFactory()
{
    return m_socketFactory;
}

Protocol * HttpServer::createProtocol()
{
    return new HttpProtocol(this);
}

void HttpServer::handleRequest(HttpProtocol * protocol, const HttpRequest & request)
{
    if (request.getHeader(http::Upgrade).getValue() == "websocket")
    {
        protocol->send(WebsocketProtocol::handshake(request));

        protocol->connection()->setProtocol(new WebsocketProtocol(this));
    }
    else
    {
        HttpResponse response = m_callback(request);

        protocol->send(response);

        if (!response.isKeepAlive())
        {
            protocol->disconnect();
        }
    }
}

void HttpServer::handleBadRequest(HttpProtocol * protocol)
{
    protocol->send(HttpResponse(http::BadRequest));
}

void HttpServer::handleText(WebsocketProtocol * protocol, const QByteArray & text)
{
    protocol->sendText(text);

    protocol->sendPing();
}

void HttpServer::handleBinary(WebsocketProtocol * protocol, const QByteArray & binary)
{
    protocol->sendBinary(binary);
}

} // namespace tastefulserver
