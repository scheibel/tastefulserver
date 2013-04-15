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

#include <TcpServer>

ThreadPool TcpServer::threadPool;
int TcpServer::serverCount = 0;

TcpServer::TcpServer() {
	serverCount++;
	if (!threadPool.isStarted()) {
		threadPool.start();
	}
}

TcpServer::~TcpServer()
{
	serverCount--;
	if (serverCount<=0) threadPool.stop();
}

void TcpServer::setNumThreads(int numThreads) {
	threadPool.setNumThreads(numThreads);
}

void TcpServer::incomingConnection(int socketDescriptor) {
	threadPool.addTask(createConnectionHandler(socketDescriptor));
}
