#include <httpUtil>
#include <httpStatusCodes>
#include <QHash>

using namespace http;

namespace internal {
	
QHash<unsigned, QString> makeReasonHash() {
	QHash<unsigned, QString> hash;
	
	hash.insert(Continue, "Continue");
	hash.insert(SwitchingProtocols, "Switching Protocols");
	hash.insert(OK, "OK");
	hash.insert(Created, "Created");
	hash.insert(Accepted, "Accepted");
	hash.insert(NonAuthoritativeInformation, "Non-Authoritative Information");
	hash.insert(NoContent, "No Content");
	hash.insert(ResetContent, "Reset Content");
	hash.insert(PartialContent, "Partial Content");
	
	hash.insert(MultipleChoices, "Multiple Choices");
	hash.insert(MovedPermanently, "Moved Permanently");
	hash.insert(Found, "Found");
	hash.insert(SeeOther, "See Other");
	hash.insert(NotModified, "Not Modified");
	hash.insert(UseProxy, "Use Proxy");
	hash.insert(TemporaryRedirect, "Temporary Redirect");
	
	hash.insert(BadRequest, "Bad Request");
	hash.insert(Unauthorized, "Unauthorized");
	hash.insert(PaymentRequired, "Payment Required");
	hash.insert(Forbidden, "Forbidden");
	hash.insert(NotFound, "Not Found");
	hash.insert(MethodNotAllowed, "Method Not Allowed");
	hash.insert(NotAcceptable, "Not Acceptable");
	hash.insert(ProxyAuthenticationRequired, "Proxy Authentication Required");
	hash.insert(RequestTimeout, "Request Time-out");
	hash.insert(Conflict, "Conflict");
	hash.insert(Gone, "Gone");
	hash.insert(LengthRequired, "Length Required");
	hash.insert(PreconditionFailed, "PreconditionFailed");
	hash.insert(RequestEntityTooLarge, "Request Entity Too Large");
	hash.insert(RequestURITooLarge, "Request-URI Too Large");
	hash.insert(UnsupportedMediaType, "Unsupported Media Type");
	hash.insert(RequestedRangeNotSatisfiable, "Requested range not satisfiable");
	hash.insert(ExpectationFailed, "Expectation Failed");
	
	hash.insert(InternalServerError, "Internal Server Error");
	hash.insert(NotImplemented, "Not Implemented");
	hash.insert(BadGateway, "Bad Gateway");
	hash.insert(ServiceUnavailable, "Service Unavailable");
	hash.insert(InternalServerError, "Gateway Time-out");
	hash.insert(HTTPVersionNotSupported, "HTTP Version not supported");
	
	return hash;
}

}

QString http::reason(const unsigned statusCode) {
	static QHash<unsigned, QString> hash = internal::makeReasonHash();
	return hash[statusCode];
}

QString http::dateString(const QDateTime& dateTime) {
	return dateTime.toUTC().toString("ddd, dd MMM yyyy hh:mm:ss 'GMT'");
}
