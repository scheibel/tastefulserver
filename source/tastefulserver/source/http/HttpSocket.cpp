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

#include <tastefulserver/HttpSocket.h>
#include <tastefulserver/Connection.h>
#include <tastefulserver/HttpSocketHandler.h>

namespace tastefulserver {

HttpSocket::HttpSocket(HttpSocketHandler * handler)
: m_handler(handler)
{
    connect(&m_parser, &HttpRequestParser::badRequest, this, &HttpSocket::badRequest);
}

void HttpSocket::send(const HttpResponse & response)
{
    sendData(response.toByteArray());

    if (!response.isKeepAlive())
    {
        disconnect();
    }
}

void HttpSocket::disconnect()
{
    AbstractSocket::disconnect();
}

void HttpSocket::receiveData(const QByteArray & data)
{
    m_parser.addData(data);

    while (m_parser.hasReadyRequests())
    {
        HttpRequest request = m_parser.popReadyRequest();
        addConnectionInfo(request);

        if (request.hasHeader(http::Upgrade))
        {
            if (m_handler->handleUpgrade(this, request))
                break;
        }

        m_handler->handleRequest(this, request);
    }
}

void HttpSocket::addConnectionInfo(HttpRequest & request)
{
    if (m_connection->isSslConnection())
    {
        request.setHttps(true);
    }

    request.setAddress(m_connection->socket().peerAddress());
    request.setPort(m_connection->socket().peerPort());
}

void HttpSocket::badRequest()
{
    m_handler->handleBadRequest(this);
}

} // namespace tastefulserver
