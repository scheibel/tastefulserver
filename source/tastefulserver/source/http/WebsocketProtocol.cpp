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

#include <tastefulserver/HttpHeader.h>

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

WebsocketProtocol::WebsocketProtocol()
{
}

void WebsocketProtocol::receive(const QByteArray & data)
{
    m_parser.addData(data);
}


} // namespace tastefulserver
