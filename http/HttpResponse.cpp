#include <HttpResponse>
#include <httpUtil>
#include <QTextStream>


HttpResponse::HttpResponse() {
	setStatusCode(http::NotFound);
	setHeader(http::Date, http::dateString(QDateTime::currentDateTime()));
	setHeader(http::Connection, "keep-alive");
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