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

#include <QObject>
#include <QAbstractSocket>

#include <tastefulserver/tastefulserver_api.h>

namespace tastefulserver {

class Connection;

class TASTEFULSERVER_API AbstractSocket : public QObject
{
    friend class Connection;

    Q_OBJECT

public:
    AbstractSocket();
    virtual ~AbstractSocket();

    Connection * connection();

    void create(qintptr socketDescriptor);

    void takeOver(AbstractSocket * socket);

signals:
    void disconnected();

private slots:
    void socketDisconnected();
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError e);

protected:
    Connection * m_connection;
    QAbstractSocket * m_socket;

    void connectSocket();
    void disconnectSocket();

    void setConnection(Connection * connection);

    void sendData(const QByteArray & data);
    void disconnect();

    virtual void onDisconnect();
    virtual void onError(QAbstractSocket::SocketError e);
    virtual void receiveData(const QByteArray & data) = 0;

    virtual QAbstractSocket * createSocket(qintptr socketDescriptor) = 0;
};

} // namespace tastefulserver
