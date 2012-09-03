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

#include <HttpResponse>
#include <httpUtil>
#include <QTextStream>


HttpResponse::HttpResponse() {
	initialize();
}

HttpResponse::HttpResponse(HttpVersion httpVersion) : HttpMessage(httpVersion) {
	initialize();
}

HttpResponse::HttpResponse(HttpRequest& httpRequest) : HttpMessage(httpRequest.getHttpVersion()) {
	initialize();
	if (httpRequest.isKeepAlive()) keepAlive();
}

void HttpResponse::initialize() {
	setStatusCode(http::NotFound);
	setHeader(http::Date, http::dateString(QDateTime::currentDateTime()));
	setHeader(http::ContentLength, "0");
}

void HttpResponse::setStatusCode(unsigned statusCode) {
	this->statusCode = statusCode;
}

unsigned HttpResponse::getStatusCode() {
	return statusCode;
}

void HttpResponse::writeHeadersOn(QTextStream& stream) {
	HttpMessage::writeHeadersOn(stream);
	for (Cookie& cookie: cookies) {
		HttpHeader header(http::SetCookie, cookie.toString());
		writeHeaderOn(header, stream);
	}
	HttpHeader header(http::ContentType, contentType.toString());
	writeHeaderOn(header, stream);
}

QByteArray HttpResponse::toByteArray() {
	QByteArray byteArray;
	QTextStream stream(&byteArray);
	
	stream << httpVersion.toString() << " " << statusCode;
	QString reason = http::reason(statusCode);
	if (!reason.isNull()) stream << " " << reason;
	stream << http::Linebreak;

	stream << HttpMessage::toByteArray();
	
	return byteArray;
}