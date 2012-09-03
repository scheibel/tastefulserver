#include <HttpDataHandler>
#include <HttpHeader>

#include <QStringList>

#include <QDebug>

HttpDataHandler::HttpDataHandler(RequestCallback callback, bool isHttps) : callback(callback), isHttps(isHttps), buffer(ByteArrayStream::forLinebreak(http::Linebreak)) {
	state = READ_REQUEST_LINE;
}

void HttpDataHandler::receive(QByteArray data) {
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

bool HttpDataHandler::readRequestLine() {
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
	
	request = HttpRequest(method, requestUri, httpVersion, isHttps);
	
	state = READ_HEADER;
	
	return true;
}

bool HttpDataHandler::readHeader() {
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

bool HttpDataHandler::readContent() {
	int length = request.getHeader(http::ContentLength).getValue().toInt();
	if (buffer.availableBytes()<length) return false;
	QByteArray content = buffer.read(length);
	request.parseContent(content);
	state = HANDLE_REQUEST;
	return true;
}

bool HttpDataHandler::handleRequest() {
	HttpResponse response = callback(request);
	
	send(response.toByteArray());
	buffer.flush();
	state = READ_REQUEST_LINE;
	return false;
}

bool HttpDataHandler::handleError() {
	request.markBad();
	
	HttpResponse response;
	response.setStatusCode(http::BadRequest);
	send(response.toByteArray());
	
	state = READ_REQUEST_LINE;
	return true;
}
