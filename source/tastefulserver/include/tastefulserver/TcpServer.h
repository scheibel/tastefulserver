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

#include <QTcpServer>

#include <tastefulserver/tastefulserver_api.h>
#include <tastefulserver/Connection.h>

namespace tastefulserver {

class ThreadPool;

class TASTEFULSERVER_API TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TcpServer();
    virtual ~TcpServer();

    static void setNumThreads(int numThreads);

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

    virtual AbstractSocket * createSocket() = 0;

    static ThreadPool * s_threadPool;
    static int s_serverCount;
};

} // namespace tastefulserver
