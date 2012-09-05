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

#include <HttpRequest>
#include <QTextStream>


HttpRequest::HttpRequest() : bad(true) {
}

HttpRequest::HttpRequest(const HttpMethod& method, const QString& requestUri, const HttpVersion& httpVersion, bool isHttps) : HttpMessage(httpVersion), bad(false), method(method), requestUri(requestUri) {
	url = QUrl::fromEncoded(requestUri.toAscii());
	if (url.scheme().isEmpty()) url.setScheme(isHttps ? "https" : "http");
	requestParams.parseUrl(url);
}

bool HttpRequest::isBad() const {
	return bad;
}

void HttpRequest::markBad() {
	bad = true;
}

bool HttpRequest::isXMLHttpRequest() const {
	return getHeader(http::XRequestedWith).getValue()=="XMLHttpRequest";
}

HttpMethod HttpRequest::getMethod() const {
	return method;
}

QString HttpRequest::getRequestUri() const {
	return requestUri;
}

void HttpRequest::parseHeader(const HttpHeader& header) {
	addHeader(header);
	QString headerName = header.getName();
	QString value = header.getValue();
	if (headerName==http::Host) {
		url.setAuthority(value);
	} else if (headerName==http::Cookie) {
		cookies.parse(value);
	} else if (headerName==http::ContentType) {
		contentType.parse(value);
	}
}

void HttpRequest::parseContent(const QByteArray& content) {
	setContent(content);
	if (isMultiPart()) {
		multiPart = MultiPart(contentType);
		multiPart.parse(content);
		if (multiPart.isFormData()) {
			requestParams.parseMultiPart(multiPart);
		}
	} else if (contentType.is(ContentType::Application, ContentType::XWWWFormUrlEncoded)) {
		requestParams.parseUrlEncoded(content);
	}
}

QUrl HttpRequest::getUrl() const {
	return url;
}

QString HttpRequest::getPath() const {
	return url.path();
}

RequestParameters& HttpRequest::getParameters() {
	return requestParams;
}

const RequestParameters& HttpRequest::getParameters() const {
	return requestParams;
}

QByteArray HttpRequest::toByteArray() const {
	QByteArray byteArray;
	
	QTextStream stream(&byteArray);
	
	stream << method.toString() << " " << requestUri << " " << httpVersion.toString() << http::Linebreak;
	
	stream << HttpMessage::toByteArray();
	
	return byteArray;
}
