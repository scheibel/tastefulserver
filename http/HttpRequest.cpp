#include <HttpRequest>
#include <QTextStream>


HttpRequest::HttpRequest() : bad(true) {
}

HttpRequest::HttpRequest(HttpMethod method, QString requestUri, HttpVersion httpVersion, bool isHttps) : HttpMessage(httpVersion), bad(false), method(method), requestUri(requestUri) {
	url = QUrl::fromEncoded(requestUri.toAscii());
	if (url.scheme().isEmpty()) url.setScheme(isHttps ? "https" : "http");
	requestParams.parseUrl(url);
}

bool HttpRequest::isBad() {
	return bad;
}

void HttpRequest::markBad() {
	bad = true;
}

bool HttpRequest::isXMLHttpRequest() {
	return getHeader(http::XRequestedWith).getValue()=="XMLHttpRequest";
}

HttpMethod HttpRequest::getMethod() {
	return method;
}

QString HttpRequest::getRequestUri() {
	return requestUri;
}

void HttpRequest::parseHeader(HttpHeader header) {
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

void HttpRequest::parseContent(QByteArray content) {
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

QUrl HttpRequest::getUrl() {
	return url;
}

QString HttpRequest::getPath() {
	return url.path();
}

RequestParameters& HttpRequest::getParameters() {
	return requestParams;
}

QByteArray HttpRequest::toByteArray() {
	QByteArray byteArray;
	
	QTextStream stream(&byteArray);
	
	stream << method.toString() << " " << requestUri << " " << httpVersion.toString() << http::Linebreak;
	
	stream << HttpMessage::toByteArray();
	
	return byteArray;
}
