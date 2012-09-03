#include <HttpMessage>
#include <QTextStream>

HttpMessage::HttpMessage() : httpVersion(HttpVersion(1,1)) {
}

HttpMessage::HttpMessage(HttpVersion httpVersion) : httpVersion(httpVersion) {
}

HttpVersion HttpMessage::getHttpVersion() {
	return httpVersion;
}

Cookies& HttpMessage::getCookies() {
	return cookies;
}

ContentType& HttpMessage::getContentType() {
	return contentType;
}

bool HttpMessage::isMultiPart() {
	return contentType.isMultiPart();
}

MultiPart& HttpMessage::getMultiPart() {
	return multiPart;
}