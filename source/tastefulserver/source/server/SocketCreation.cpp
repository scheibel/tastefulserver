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

#include "SocketCreation.h"

namespace tastefulserver {

SocketCreation::SocketCreation(qintptr socketDescriptor)
    : socketDescriptor(socketDescriptor)
{
}

SocketCreation::~SocketCreation()
{
}

bool SocketCreation::isTcp() const
{
    return false;
}

bool SocketCreation::isSsl() const
{
    return false;
}

bool SocketCreation::isUdp() const
{
    return false;
}

UdpSocketCreation::UdpSocketCreation(qintptr socketDescriptor)
    : SocketCreation(socketDescriptor)
{
}

QAbstractSocket * UdpSocketCreation::operator()() const
{
    QUdpSocket * socket = new QUdpSocket();

    socket->setSocketDescriptor(socketDescriptor);

    return socket;
}

bool UdpSocketCreation::isUdp() const
{
    return true;
}

TcpSocketCreation::TcpSocketCreation(qintptr socketDescriptor)
    : SocketCreation(socketDescriptor)
{
}

QAbstractSocket * TcpSocketCreation::operator()() const
{
    QTcpSocket * socket = new QTcpSocket();

    socket->setSocketDescriptor(socketDescriptor);

    return socket;
}

bool TcpSocketCreation::isTcp() const
{
    return true;
}

SslSocketCreation::SslSocketCreation(qintptr socketDescriptor, const QSslCertificate & certificate, const QSslKey & privateKey)
    : TcpSocketCreation(socketDescriptor)
    , certificate(certificate)
    , privateKey(privateKey)
{
}

QAbstractSocket * SslSocketCreation::operator()() const
{
    QSslSocket * socket = new QSslSocket();

    socket->setSocketDescriptor(socketDescriptor);

    socket->setLocalCertificate(certificate);
    socket->setPrivateKey(privateKey);

    socket->startServerEncryption();

    return socket;
}

bool SslSocketCreation::isSsl() const
{
    return true;
}

} // namespace tastefulserver
