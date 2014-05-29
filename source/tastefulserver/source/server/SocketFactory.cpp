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

#include <tastefulserver/SocketFactory.h>

#include <QUdpSocket>
#include <QTcpSocket>
#include <QSslSocket>

namespace tastefulserver {

SocketFactory::SocketFactory()
{
}

SocketFactory::~SocketFactory()
{
}

bool SocketFactory::isTcp() const
{
    return false;
}

bool SocketFactory::isSsl() const
{
    return false;
}

bool SocketFactory::isUdp() const
{
    return false;
}

UdpSocketFactory::UdpSocketFactory()
{
}

QAbstractSocket * UdpSocketFactory::operator()(qintptr socketDescriptor) const
{
    QUdpSocket * socket = new QUdpSocket();

    socket->setSocketDescriptor(socketDescriptor);

    return socket;
}

bool UdpSocketFactory::isUdp() const
{
    return true;
}

TcpSocketFactory::TcpSocketFactory()
{
}

QAbstractSocket * TcpSocketFactory::operator()(qintptr socketDescriptor) const
{
    QTcpSocket * socket = new QTcpSocket();

    socket->setSocketDescriptor(socketDescriptor);

    return socket;
}

bool TcpSocketFactory::isTcp() const
{
    return true;
}

SslSocketFactory::SslSocketFactory(const QSslCertificate & certificate, const QSslKey & privateKey)
: m_certificate(certificate)
, m_privateKey(privateKey)
{
}

QAbstractSocket * SslSocketFactory::operator()(qintptr socketDescriptor) const
{
    QSslSocket * socket = new QSslSocket();

    socket->setSocketDescriptor(socketDescriptor);

    socket->setLocalCertificate(m_certificate);
    socket->setPrivateKey(m_privateKey);

    socket->startServerEncryption();

    return socket;
}

bool SslSocketFactory::isSsl() const
{
    return true;
}

} // namespace tastefulserver
