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

#include <HttpMessage>
#include <QTextStream>

HttpMessage::HttpMessage() : httpVersion(HttpVersion(1,1)) {
}

HttpMessage::HttpMessage(HttpVersion httpVersion) : httpVersion(httpVersion) {
}

HttpVersion& HttpMessage::getHttpVersion() {
	return httpVersion;
}

Cookies& HttpMessage::getCookies() {
	return cookies;
}

bool HttpMessage::hasCookies() {
	return !cookies.isEmpty();
}

ContentType& HttpMessage::getContentType() {
	return contentType;
}

void HttpMessage::keepAlive() {
	setHeader(http::Connection, "Keep-Alive");
}

bool HttpMessage::isKeepAlive() {
	return getHeader(http::Connection).getValue().toLower()=="keep-alive";
}

int HttpMessage::getContentLength() {
	return getHeader(http::ContentLength).getValue().toInt();
}

bool HttpMessage::isMultiPart() {
	return contentType.isMultiPart();
}

MultiPart& HttpMessage::getMultiPart() {
	return multiPart;
}