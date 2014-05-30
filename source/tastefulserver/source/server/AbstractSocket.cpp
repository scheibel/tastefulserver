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

#include <tastefulserver/AbstractSocket.h>
#include <tastefulserver/Connection.h>

#include <QDebug>

namespace tastefulserver {

using ErrorSignal =void (QAbstractSocket::*)(QAbstractSocket::SocketError);

AbstractSocket::AbstractSocket()
: m_connection(nullptr)
, m_socket(nullptr)
{
}

AbstractSocket::~AbstractSocket()
{
    delete m_socket;
}

void AbstractSocket::create(qintptr socketDescriptor)
{
    if (m_socket)
    {
        return;
    }

    m_socket = createSocket(socketDescriptor);

    connectSocket();
}

void AbstractSocket::connectSocket()
{
    connect(m_socket, &QAbstractSocket::readyRead, this, &AbstractSocket::socketReadyRead);
    connect(m_socket, &QAbstractSocket::disconnected, this, &AbstractSocket::socketDisconnected);
    connect(m_socket, static_cast<ErrorSignal>(&QAbstractSocket::error), this, &AbstractSocket::socketError);
}

void AbstractSocket::disconnectSocket()
{
    QObject::disconnect(m_socket, &QAbstractSocket::readyRead, this, &AbstractSocket::socketReadyRead);
    QObject::disconnect(m_socket, &QAbstractSocket::disconnected, this, &AbstractSocket::socketDisconnected);
    QObject::disconnect(m_socket, static_cast<ErrorSignal>(&QAbstractSocket::error), this, &AbstractSocket::socketError);
}

void AbstractSocket::takeOverFrom(AbstractSocket * socket)
{
    if (!socket->m_socket)
    {
        return;
    }

    if (m_socket)
    {
        disconnectSocket();
        delete m_socket;
        m_socket = nullptr;
    }

    socket->disconnectSocket();

    m_socket = socket->m_socket;
    socket->m_socket = nullptr;

    connectSocket();

    socket->connection()->setSocket(this);
}

void AbstractSocket::socketDisconnected()
{
    onDisconnect();
    emit(disconnected());
}

void AbstractSocket::socketReadyRead()
{
    receiveData(m_socket->readAll());
}

void AbstractSocket::socketError(QAbstractSocket::SocketError e)
{
    onError(e);
}

void AbstractSocket::setConnection(Connection * connection)
{
    m_connection = connection;
}

Connection * AbstractSocket::connection()
{
    return m_connection;
}

void AbstractSocket::sendData(const QByteArray & data)
{
    m_socket->write(data);
}

void AbstractSocket::onError(QAbstractSocket::SocketError e)
{
    if (e != QAbstractSocket::RemoteHostClosedError)
    {
        qDebug() << "Socket error: " << m_socket->errorString();
    }
}

void AbstractSocket::disconnect()
{
    m_socket->disconnectFromHost();
}

void AbstractSocket::onDisconnect()
{
}

} // namespace tastefulserver
