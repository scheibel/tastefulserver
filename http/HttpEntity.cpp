#include <HttpEntity>
#include <QTextStream>

HttpEntity::HttpEntity() {
}

bool HttpEntity::hasHeader(http::HeaderName headerName) {
	return headers.contains(headerName);
}

HttpHeader HttpEntity::getHeader(http::HeaderName headerName) {
	QList<HttpHeader>& list = headers[headerName];
	if (list.isEmpty()) {
		return HttpHeader::forName(headerName);
	}
	return list.first();
}

QList<HttpHeader> HttpEntity::getHeaders(http::HeaderName headerName) {
	return headers[headerName];
}

void HttpEntity::addHeaders(QList<HttpHeader> headers) {
	for (HttpHeader& header: headers) {
		addHeader(header);
	}
}

void HttpEntity::addHeader(HttpHeader header) {
	headers[header.getName()] << header;
}

void HttpEntity::setHeader(http::HeaderName headerName, QString value, bool merge) {
	setHeader(HttpHeader(headerName, value), merge);
}

void HttpEntity::setHeader(HttpHeader header, bool merge) {
	QList<HttpHeader>& list = headers[header.getName()];
	if (list.isEmpty()) {
		list << header;
	} else {
		if (merge) {
			list.first().merge(header);
		} else {
			list[0] = header;
		}
	}
}

void HttpEntity::removeHeader(http::HeaderName headerName) {
	headers.remove(headerName);
}

QByteArray HttpEntity::getContent() {
	return content;
}

void HttpEntity::clearContent() {
	content.clear();
	removeHeader(http::ContentLength);
}

void HttpEntity::setContent(QByteArray content) {
	this->content = content;
	setHeader(http::ContentLength, QString::number(content.size()));
}

void HttpEntity::writeHeaderOn(HttpHeader& header, QTextStream& stream) {
	if (header.isValid()) stream << header.toString() << http::Linebreak;
}

void HttpEntity::writeHeadersOn(QTextStream& stream) {
	for (QList<HttpHeader>& list: headers) {
		for (HttpHeader& header: list) {
			writeHeaderOn(header, stream);
		}
	}	
}

QByteArray HttpEntity::toByteArray() {
	QByteArray byteArray;
	
	QTextStream stream(&byteArray);

	writeHeadersOn(stream);
	
	stream << http::Linebreak;
	
	if (!content.isNull()) {
		stream << content;
	}
	
	return byteArray;
}
