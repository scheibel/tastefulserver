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

#include <internal/ConnectionHandler>
#include <internal/ByteArrayStream>
#include <http>

using namespace internal;

class HttpHandler : public ConnectionHandler {
	public:
		typedef std::function<HttpResponse(HttpRequest&)> RequestCallback;
		
		HttpHandler(const RequestCallback& callback);
	
		void receive(const QByteArray& data);
	private:		
		bool readRequestLine();
		bool readHeader();
		bool readContent();
		bool handleRequest();
		bool handleError();
		
		RequestCallback callback;
		ByteArrayStream buffer;
		HttpRequest request;
		enum {
			READ_REQUEST_LINE,
			READ_HEADER,
			READ_CONTENT,
			HANDLE_REQUEST,
			HANDLE_ERROR
		} state;
};
