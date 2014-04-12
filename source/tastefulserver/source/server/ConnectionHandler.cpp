/**
 * (C) LGPL-3
 *
 * Tasteful Server <https://github.com/scheibel/tasteful-server>
 *
 * Copyright: 2012 Lux, Scheibel
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
    : _socket(nullptr)
    , socketCreation(nullptr)
{
}

ConnectionHandler::ConnectionHandler(SocketCreation * socketCreation)
    : _socket(nullptr)
    , socketCreation(socketCreation)
{
}

ConnectionHandler::~ConnectionHandler()
{
    delete _socket;
    delete socketCreation;
}

void ConnectionHandler::setSocketCreator(SocketCreation * socketCreation)
{
    delete this->socketCreation;
    this->socketCreation = socketCreation;
}

void ConnectionHandler::startUp()
{
    createSocket();

    QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    QObject::connect(_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}

void ConnectionHandler::createSocket()
{
    _socket = (*socketCreation)();
}

QAbstractSocket &ConnectionHandler::socket()
{
    return *_socket;
}

bool ConnectionHandler::isUdpConnection() const
{
    return socketCreation->isUdp();
}

bool ConnectionHandler::isTcpConnection() const
{
    return socketCreation->isTcp();
}

bool ConnectionHandler::isSslConnection() const
{
    return socketCreation->isSsl();
}

void ConnectionHandler::disconnected()
{
    onDisconnect();
    finish();
}

void ConnectionHandler::readyRead()
{
    receive(_socket->readAll());
}

void ConnectionHandler::send(const QByteArray & data)
{
    _socket->write(data);
}

void ConnectionHandler::error(QAbstractSocket::SocketError e)
{
    onError(e);
}

void ConnectionHandler::onError(QAbstractSocket::SocketError e)
{
    if (e!=QAbstractSocket::RemoteHostClosedError)
    {
        qDebug() << "Socket error: " << _socket->errorString();
    }
}

void ConnectionHandler::disconnect()
{
    _socket->disconnectFromHost();
}

void ConnectionHandler::onDisconnect()
{
}

} // namespace tastefulserver
