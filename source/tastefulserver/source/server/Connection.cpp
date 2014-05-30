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

#include <tastefulserver/Connection.h>
#include <tastefulserver/AbstractSocket.h>

#include <QDebug>

namespace tastefulserver {

Connection::Connection(qintptr socketDescriptor)
: m_socketDescriptor(socketDescriptor)
, m_socket(nullptr)
{
}

Connection::~Connection()
{
    delete m_socket;
}

void Connection::setSocket(AbstractSocket * socket)
{
    if (m_socket)
    {
        disconnect(m_socket, &AbstractSocket::disconnected, this, &Connection::disconnected);
        m_socket->deleteLater();
        m_socket->setConnection(nullptr);
        m_socket->setParent(nullptr);
    }


    m_socket = socket;
    m_socket->setConnection(this);
    m_socket->setParent(this); // so that socket will be moved to the same thread as this
    connect(m_socket, &AbstractSocket::disconnected, this, &Connection::disconnected);
}

void Connection::startUp()
{
    m_socket->create(m_socketDescriptor);
}

void Connection::disconnected()
{
    finish();
}


} // namespace tastefulserver
