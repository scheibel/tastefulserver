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

#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/Task.h>

#include <QAbstractSocket>

namespace tastefulserver {

class SocketCreation;
	
class TASTEFULSERVER_API ConnectionHandler : public Task {
    Q_OBJECT
	public:
		ConnectionHandler();
        ConnectionHandler(SocketCreation* socketCreation);
		~ConnectionHandler();
	
        void setSocketCreator(SocketCreation* socketCreation);
	
		void startUp();
	protected:
		QAbstractSocket* _socket;
	private:
        SocketCreation* socketCreation;
		void createSocket();
	private slots:
		void disconnected();
		void readyRead();
		void error(QAbstractSocket::SocketError e);
	protected:
		QAbstractSocket& socket();
	
		bool isUdpConnection() const;
		bool isTcpConnection() const;
		bool isSslConnection() const;
	
		void send(const QByteArray& data);
		void disconnect();
	
		virtual void onDisconnect();
		virtual void onError(QAbstractSocket::SocketError e);
	
		virtual void receive(const QByteArray& data) = 0;
};

} // namespace tastefulserver
