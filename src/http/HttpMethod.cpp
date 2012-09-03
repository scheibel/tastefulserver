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