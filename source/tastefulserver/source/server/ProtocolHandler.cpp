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

#include <tastefulserver/ProtocolHandler.h>
#include <tastefulserver/ConnectionHandler.h>

#include <QDebug>

namespace tastefulserver {

ProtocolHandler::ProtocolHandler()
: m_connection(nullptr)
{
}

ProtocolHandler::~ProtocolHandler()
{
}

void ProtocolHandler::setConnection(Connection * connection)
{
    m_connection = connection;
}

Connection * ProtocolHandler::connection()
{
    return m_connection;
}

void ProtocolHandler::send(const QByteArray & data)
{
    m_connection->send(data);
}

void ProtocolHandler::onError(QAbstractSocket::SocketError e)
{
    if (e != QAbstractSocket::RemoteHostClosedError)
    {
        qDebug() << "Socket error: " << m_connection->socket().errorString();
    }
}

void ProtocolHandler::disconnect()
{
    m_connection->disconnect();
}

void ProtocolHandler::onDisconnect()
{
}

} // namespace tastefulserver
