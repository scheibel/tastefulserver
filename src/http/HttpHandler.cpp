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

#include <HttpHandler>
#include <HttpHeader>

#include <QStringList>

#include <QDebug>

HttpHandler::HttpHandler(const RequestCallback& callback) : callback(callback), buffer(ByteArrayStream::forLinebreak(http::Linebreak)) {
	state = READ_REQUEST_LINE;
}

void HttpHandler::receive(const QByteArray& data) {
	buffer.append(data);
	
	bool continueReading = true;
	
	while (continueReading) {
		switch (state) {
			case READ_REQUEST_LINE:
				continueReading = readRequestLine();
				break;
			case READ_HEADER:
				continueReading = readHeader();
				break;
			case READ_CONTENT:
				continueReading = readContent();
				break;
			case HANDLE_REQUEST:
				continueReading = handleRequest();
				break;
			case HANDLE_ERROR:
				continueReading = handleError();
				break;
		}
	}
}

bool HttpHandler::readRequestLine() {
	if (!buffer.canReadLine()) return false;
	
	QString line = buffer.readLine();
	
	if (line.isEmpty()) return true;
	
	QStringList parts = line.split(' ');
	if (parts.size()<3) {
		state = HANDLE_ERROR;
		return true;
	}
	HttpMethod method = HttpMethod::fromString(parts[0]);
	HttpVersion httpVersion = HttpVersion::fromString(parts[2]);
	
	if (method.isInvalid() || httpVersion.isInvalid()) {
		state = HANDLE_ERROR;
		return true;
	}
	
	QString requestUri = parts[1];
	
	request = HttpRequest(method, requestUri, httpVersion, isSslConnection());
	
	state = READ_HEADER;
	
	return true;
}

bool HttpHandler::readHeader() {
	if (!buffer.canReadLine()) return false;
	
	QString line = buffer.readLine();
	if (line.isEmpty()) {
		state = request.hasHeader(http::ContentLength) ? READ_CONTENT : HANDLE_REQUEST;
		return true;
	} else {
		int pos = line.indexOf(": ");
		if (pos<0) {
			request.markBad();
			return true;
		}
		
		QString fieldName = line.left(pos);
		QString fieldValue = line.mid(pos+2);
		
		HttpHeader header(fieldName, fieldValue);
		request.parseHeader(header);
	}
	
	return true;
}

bool HttpHandler::readContent() {
	int length = request.getContentLength();
	if (buffer.availableBytes()<length) return false;
	QByteArray content = buffer.read(length);
	request.parseContent(content);
	state = HANDLE_REQUEST;
	return true;
}

bool HttpHandler::handleRequest() {
	HttpResponse response = callback(request);
	
	send(response.toByteArray());
	buffer.flush();
	state = READ_REQUEST_LINE;
	
	if (!response.isKeepAlive()) {
		disconnect();
	}
	
	return false;
}

bool HttpHandler::handleError() {
	request.markBad();
	
	HttpResponse response;
	response.setStatusCode(http::BadRequest);
	send(response.toByteArray());
	
	state = READ_REQUEST_LINE;
	return true;
}
