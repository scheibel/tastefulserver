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

#pragma once

#include <QAbstractSocket>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QSslSocket>
#include <QSslCertificate>
#include <QSslKey>

namespace tastefulserver {

class SocketCreation
{
public:
    SocketCreation(qintptr socketDescriptor);
    virtual ~SocketCreation();

    virtual QAbstractSocket * operator()() const = 0;
    virtual bool isTcp() const;
    virtual bool isSsl() const;
    virtual bool isUdp() const;

protected:
    qintptr m_socketDescriptor;
};

class UdpSocketCreation : public SocketCreation
{
public:
    UdpSocketCreation(qintptr socketDescriptor);
    virtual QAbstractSocket * operator()() const;
    virtual bool isUdp() const;
};

class TcpSocketCreation : public SocketCreation
{
public:
    TcpSocketCreation(qintptr socketDescriptor);
    virtual QAbstractSocket * operator()() const;
    virtual bool isTcp() const;
};

class SslSocketCreation : public TcpSocketCreation
{
public:
    SslSocketCreation(qintptr socketDescriptor, const QSslCertificate & certificate, const QSslKey & privateKey);
    virtual QAbstractSocket * operator()() const;
    virtual bool isSsl() const;

protected:
    QSslCertificate m_certificate;
    QSslKey m_privateKey;
};

} // namespace tastefulserver
