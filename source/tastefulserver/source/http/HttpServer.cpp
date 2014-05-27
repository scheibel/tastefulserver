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

namespace tastefulserver {

HttpServer::HttpServer(const RequestCallback & callback)
: m_callback(callback)
{
}

SocketFactory * HttpServer::createSocketFactory(qintptr socketDescriptor)
{
    return new TcpSocketFactory(socketDescriptor);
}

Connection * HttpServer::createConnection() const
{
    return new Connection(
        new HttpProtocol([this](const HttpRequest & request, HttpProtocol & protocol) {
            return handle(request, protocol);
        })
    );
}

bool HttpServer::handle(const HttpRequest & request, HttpProtocol & protocol)
{
    if (request.isBad())
    {
        HttpResponse response;
        response.setStatusCode(http::BadRequest);

        protocol.sendResponse(response);
    }
    else if (request.getHeader(http::Upgrade).getValue() == "websocket")
    {
        protocol.sendResponse(WebsocketProtocol::handshake(request));
        protocol.connection()->setProtocol(new WebsocketProtocol());
        return false;
    }
    else
    {
        protocol.sendResponse(m_callback(request));
    }

    return true;
}

} // namespace tastefulserver
