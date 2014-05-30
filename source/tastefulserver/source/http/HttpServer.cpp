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
#include <tastefulserver/WebSocket.h>

#include <QTimer>

namespace tastefulserver {

HttpServer::HttpServer(const RequestCallback & callback)
: m_callback(callback)
{
}

HttpServer::~HttpServer()
{
}

AbstractSocket * HttpServer::createSocket()
{
    return new HttpSocket(this);
}

void HttpServer::handleRequest(HttpSocket * socket, const HttpRequest & request)
{
    socket->send(m_callback(request));
}

bool HttpServer::handleUpgrade(HttpSocket * socket, const HttpRequest & request)
{
    if (request.getHeader(http::Upgrade).getValue() == "websocket")
    {
        WebSocket * websocket = new WebSocket(this);

        websocket->upgrade(socket, request);

        return true;
    }

    return HttpSocketHandler::handleUpgrade(socket, request);
}

void HttpServer::handleText(WebSocket * socket, const QByteArray & text)
{
    socket->sendText(text);

    socket->sendPing();
}

void HttpServer::handleBinary(WebSocket * socket, const QByteArray & binary)
{
    socket->sendBinary(binary);
}

void HttpServer::connectionEstablished(WebSocket * socket)
{
    QTimer * timer = new QTimer(socket);
    timer->setInterval(1000);

    connect(timer, &QTimer::timeout, [socket]() {
        QString text = "hallo "+QString::number(qrand());

        qDebug() << "Send:" << text;
        socket->sendText(text.toLatin1());
    });

    timer->start();
}

} // namespace tastefulserver
