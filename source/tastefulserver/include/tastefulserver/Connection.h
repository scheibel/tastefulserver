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

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/SocketFactory.h>
#include <tastefulserver/Task.h>

namespace tastefulserver {

class SocketFactory;
class Protocol;

class TASTEFULSERVER_API Connection : public Task
{
    Q_OBJECT

public:
    Connection(qintptr socketDescriptor, SocketFactory * socketFactory);
    ~Connection();

    void setProtocol(Protocol * protocol);

    void startUp();

    QAbstractSocket & socket();
    const QAbstractSocket & socket() const;

    bool isUdpConnection() const;
    bool isTcpConnection() const;
    bool isSslConnection() const;

    void send(const QByteArray & data);
    void disconnect();

protected:
    qintptr m_socketDescriptor;
    SocketFactory * m_socketFactory;
    QAbstractSocket * m_socket;
    Protocol * m_protocol;

    void createSocket();

private slots:
    void disconnected();
    void readyRead();
    void error(QAbstractSocket::SocketError e);
};

} // namespace tastefulserver
