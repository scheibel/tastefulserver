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
#include <tastefulserver/Protocol.h>

#include <QDebug>

namespace tastefulserver {

Connection::Connection()
: m_socketFactory(nullptr)
, m_protocol(nullptr)
, m_socket(nullptr)
{
}

Connection::Connection(Protocol * protocol)
: m_socketFactory(nullptr)
, m_protocol(protocol)
, m_socket(nullptr)
{
    m_protocol->setConnection(this);
}

Connection::~Connection()
{
    delete m_protocol;
    delete m_socket;
    delete m_socketFactory;
}

void Connection::setSocketFactory(SocketFactory * socketFactory)
{
    if (m_socket)
    {
        qDebug() << "Connection already has a socket.";
        delete socketFactory;
        return;
    }

    delete m_socketFactory;
    m_socketFactory = socketFactory;
}

void Connection::setProtocol(Protocol * protocol)
{
    delete m_protocol;
    m_protocol = protocol;
}

void Connection::startUp()
{
    if (m_socket)
    {
        return;
    }

    createSocket();

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}

void Connection::createSocket()
{
    m_socket = (*m_socketFactory)();
}

QAbstractSocket & Connection::socket()
{
    return *m_socket;
}

const QAbstractSocket & Connection::socket() const
{
    return *m_socket;
}

bool Connection::isUdpConnection() const
{
    return m_socketFactory->isUdp();
}

bool Connection::isTcpConnection() const
{
    return m_socketFactory->isTcp();
}

bool Connection::isSslConnection() const
{
    return m_socketFactory->isSsl();
}

void Connection::disconnected()
{
    if (m_protocol)
    {
        m_protocol->onDisconnect();
    }

    finish();
}

void Connection::readyRead()
{
    if (m_protocol)
    {
        m_protocol->receive(m_socket->readAll());
    }
}

void Connection::send(const QByteArray & data)
{
    m_socket->write(data);
}

void Connection::error(QAbstractSocket::SocketError e)
{
    if (m_protocol)
    {
        m_protocol->onError(e);
    }
}

void Connection::disconnect()
{
    m_socket->disconnectFromHost();
}

} // namespace tastefulserver