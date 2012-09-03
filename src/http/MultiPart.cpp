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

#include <MultiPart>
#include <HttpEntity>
#include <HttpHeader>
#include <internal/ByteArrayStream>

#include<QDebug>

using namespace internal;

Part::Part() {
}

Part::Part(QList<HttpHeader> headers, QByteArray content) : content(content) {
	for (HttpHeader& header: headers) {
		this->headers.insert(header.getName(), header);
	}
}

bool Part::hasHeader(http::HeaderName headerName) {
	return headers.contains(headerName);
}

HttpHeader Part::getHeader(http::HeaderName headerName) {
	return headers[headerName];
}

QList<HttpHeader> Part::getHeaders() {
	return headers.values();
}

QByteArray Part::getContent() {
	return content;
}


MultiPart::MultiPart() {
}

MultiPart::MultiPart(ContentType type) : type(type) {
}

bool MultiPart::isFormData() {
	return type.getSubtype()==ContentType::FormData;
}

QList<Part> MultiPart::getParts() {
	return parts;
}

void MultiPart::parse(QByteArray content) {
	if (!type.isMultiPart()) {
		return;
	}
	
	QString boundary = type.getBoundary();
	if (boundary.isEmpty()) return;
	boundary = "--"+boundary;
	ByteArrayStream stream(content, http::Linebreak);
	
	stream.skipBehind(boundary);
	stream.skip(2);
	while (stream.canReadUpTo(boundary)) {
		QList<HttpHeader> headers;
		while (stream.canReadLine()) {
			QString line = stream.readLine();
			if (line.isEmpty()) break;
			headers << HttpHeader::fromString(line);
		}
		QByteArray partContent = stream.readUpTo(http::Linebreak+boundary, true);
		stream.skip(2);
		parts << Part(headers, partContent);
	}
}