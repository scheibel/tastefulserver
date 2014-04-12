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

#include <tastefulserver/HttpResponse.h>
#include <tastefulserver/httpUtil.h>

#include <QTextStream>

namespace tastefulserver {

HttpResponse::HttpResponse() {
	initialize();
}

HttpResponse::HttpResponse(const HttpVersion& httpVersion) : HttpMessage(httpVersion) {
	initialize();
}

HttpResponse::HttpResponse(const HttpRequest& httpRequest) : HttpMessage(httpRequest.getHttpVersion()) {
	initialize();
	if (httpRequest.getHttpVersion()>=HttpVersion(1,1) || httpRequest.isKeepAlive()) keepAlive();
}

void HttpResponse::initialize() {
	setStatusCode(http::NotFound);
	setHeader(http::ContentLength, "0");
	setDate(QDateTime::currentDateTime());
}

void HttpResponse::setDate(const QDateTime& date) {
	setHeader(http::Date, http::dateString(date));
}

void HttpResponse::setStatusCode(unsigned statusCode) {
	this->statusCode = statusCode;
}

unsigned HttpResponse::getStatusCode() const {
	return statusCode;
}

void HttpResponse::setCookie(const Cookie& cookie) {
	cookies.set(cookie);
}

Cookie& HttpResponse::setCookie(const QString& key, const QString& value) {
	setCookie(Cookie(key, value));
	
	return cookies[key];
}

void HttpResponse::writeHeadersOn(QTextStream& stream) const {
	HttpMessage::writeHeadersOn(stream);
	for (const Cookie& cookie: cookies) {
		HttpHeader header(http::SetCookie, cookie.toString());
		writeHeaderOn(header, stream);
	}
	HttpHeader header(http::ContentType, contentType.toString());
	writeHeaderOn(header, stream);
}

QByteArray HttpResponse::toByteArray() const {
	QByteArray byteArray;
	QTextStream stream(&byteArray);
	
	stream << httpVersion.toString() << " " << statusCode;
	QString reason = http::reason(statusCode);
	if (!reason.isNull()) stream << " " << reason;
	stream << http::Linebreak;

	stream << HttpMessage::toByteArray();
	
	return byteArray;
}

} // namespace tastefulserver
