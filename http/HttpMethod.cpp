#include <HttpMethod>

HttpMethod::HttpMethod() : _method(INVALID) {
}

HttpMethod::HttpMethod(Method method) : _method(method) {
}

HttpMethod::HttpMethod(const HttpMethod& method) : _method(method._method) {
}

HttpMethod HttpMethod::fromString(QString methodString) {
	static QHash<QString, Method> hash = makeHash();
	return HttpMethod(hash.value(methodString.toUpper(), INVALID));
}

QHash<QString, HttpMethod::Method> HttpMethod::makeHash() {
	QHash<QString, Method> hash;
	
	hash.insert("GET", GET);
	hash.insert("POST", POST);
	hash.insert("PUT", PUT);
	hash.insert("DELETE", DELETE);
	hash.insert("UPDATE", UPDATE);
	hash.insert("TRACE", TRACE);
	hash.insert("HEAD", HEAD);
	hash.insert("OPTIONS", OPTIONS);
	hash.insert("CONNECT", CONNECT);
	
	return hash;
}

HttpMethod& HttpMethod::operator=(Method method) {
	_method = method;
}

HttpMethod& HttpMethod::operator=(HttpMethod method) {
	_method = method._method;
}

bool HttpMethod::operator==(Method method) {
	return _method == method;
}
bool HttpMethod::operator==(const HttpMethod& method) {
	return _method == method._method;
}

bool HttpMethod::operator!=(Method method) {
	return _method != method;
}
bool HttpMethod::operator!=(const HttpMethod& method) {
	return _method != method._method;
}

HttpMethod::operator unsigned() {
	return _method;
}

HttpMethod::operator Method() {
	return _method;
}

bool HttpMethod::isInvalid() {
	return _method == INVALID;
}

HttpMethod::Method HttpMethod::method() {
	return _method;
}

QString HttpMethod::toString() {
	switch (_method) {
		case GET:
			return "GET";
		case POST:
			return "POST";
		case PUT:
			return "PUT";
		case DELETE:
			return "DELETE";
		case UPDATE:
			return "UPDATE";
		case TRACE:
			return "TRACE";
		case HEAD:
			return "HEAD";
		case OPTIONS:
			return "OPTIONS";
		case CONNECT:
			return "CONNECT";
		default:
			return "INVALID";
	}
}