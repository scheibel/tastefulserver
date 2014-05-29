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
#include <QSslCertificate>
#include <QSslKey>

namespace tastefulserver {

class SocketFactory
{
public:
    SocketFactory();
    virtual ~SocketFactory();

    virtual QAbstractSocket * operator()(qintptr socketDescriptor) const = 0;

    virtual bool isTcp() const;
    virtual bool isSsl() const;
    virtual bool isUdp() const;
};

class UdpSocketFactory : public SocketFactory
{
public:
    UdpSocketFactory();

    virtual QAbstractSocket * operator()(qintptr socketDescriptor) const override;
    virtual bool isUdp() const override;
};

class TcpSocketFactory : public SocketFactory
{
public:
    TcpSocketFactory();

    virtual QAbstractSocket * operator()(qintptr socketDescriptor) const override;
    virtual bool isTcp() const override;
};

class SslSocketFactory : public TcpSocketFactory
{
public:
    SslSocketFactory(const QSslCertificate & certificate, const QSslKey & privateKey);

    virtual QAbstractSocket * operator()(qintptr socketDescriptor) const override;
    virtual bool isSsl() const override;

protected:
    QSslCertificate m_certificate;
    QSslKey m_privateKey;
};

} // namespace tastefulserver
