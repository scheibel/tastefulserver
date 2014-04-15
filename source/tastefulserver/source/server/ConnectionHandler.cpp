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

#include <tastefulserver/ConnectionHandler.h>

#include "../server/SocketCreation.h"

#include <QDebug>

namespace tastefulserver {

ConnectionHandler::ConnectionHandler()
    : m_socket(nullptr)
    , m_socketCreation(nullptr)
{
}

ConnectionHandler::ConnectionHandler(SocketCreation * socketCreation)
    : m_socket(nullptr)
    , m_socketCreation(socketCreation)
{
}

ConnectionHandler::~ConnectionHandler()
{
    delete m_socket;
    delete m_socketCreation;
}

void ConnectionHandler::setSocketCreator(SocketCreation * socketCreation)
{
    delete m_socketCreation;
    m_socketCreation = socketCreation;
}

void ConnectionHandler::startUp()
{
    createSocket();

    QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    QObject::connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}

void ConnectionHandler::createSocket()
{
    m_socket = (*m_socketCreation)();
}

QAbstractSocket &ConnectionHandler::socket()
{
    return *m_socket;
}

bool ConnectionHandler::isUdpConnection() const
{
    return m_socketCreation->isUdp();
}

bool ConnectionHandler::isTcpConnection() const
{
    return m_socketCreation->isTcp();
}

bool ConnectionHandler::isSslConnection() const
{
    return m_socketCreation->isSsl();
}

void ConnectionHandler::disconnected()
{
    onDisconnect();
    finish();
}

void ConnectionHandler::readyRead()
{
    receive(m_socket->readAll());
}

void ConnectionHandler::send(const QByteArray & data)
{
    m_socket->write(data);
}

void ConnectionHandler::error(QAbstractSocket::SocketError e)
{
    onError(e);
}

void ConnectionHandler::onError(QAbstractSocket::SocketError e)
{
    if (e!=QAbstractSocket::RemoteHostClosedError)
    {
        qDebug() << "Socket error: " << m_socket->errorString();
    }
}

void ConnectionHandler::disconnect()
{
    m_socket->disconnectFromHost();
}

void ConnectionHandler::onDisconnect()
{
}

} // namespace tastefulserver
