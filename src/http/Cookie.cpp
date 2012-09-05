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

#include <Cookie>
#include <HttpHeader>
#include <QTextStream>
#include <QStringList>

const QString Cookie::MaxAge = "Max-Age";
const QString Cookie::Comment = "Comment";
const QString Cookie::Domain = "Domain";
const QString Cookie::Path = "Path";
const QString Cookie::Version = "Version";
const QString Cookie::Expires = "Expires";
const QString Cookie::Secure = "Secure";
const QString Cookie::HttpOnly = "HttpOnly";

Cookie::Cookie() : secure(false), httpOnly(false) {
}

Cookie::Cookie(const QString& name, const QString& value) : name(name), value(value), secure(false), httpOnly(false) {
}

QString Cookie::getName() const {
	return name;
}

QString Cookie::getValue() const {
	return value;
}

Cookie& Cookie::setValue(const QString& value) {
	this->value = value;
	
	return *this;
}

Cookie& Cookie::setMaxAge(unsigned seconds) {
	attributes[MaxAge] = QString::number(seconds);
	
	return *this;
}

Cookie& Cookie::setComment(const QString& comment) {
	attributes[Comment] = comment;
	
	return *this;
}
	
Cookie& Cookie::setDomain(const QString& domain) {
	attributes[Domain] = domain;
	
	return *this;
}
	
Cookie& Cookie::setPath(const QString& path) {
	attributes[Path] = path;
	
	return *this;
}
	
Cookie& Cookie::setVersion(const QString& version) {
	attributes[Version] = version;
	
	return *this;
}
	
Cookie& Cookie::setExpires(const QDateTime& dateTime) {
	return setExpires(http::dateString(dateTime));
}
	
Cookie& Cookie::setExpires(const QString& expires) {
	attributes[Expires] = expires;
	
	return *this;
}
	
Cookie& Cookie::setSecure(bool secure) {
	this->secure = secure;
	
	return *this;
}
	
Cookie& Cookie::setHttpOnly(bool httpOnly) {
	this->httpOnly = httpOnly;
	
	return *this;
}

QString Cookie::toString() const {
	if (name.isNull()) return QString();
	
	QString string;
	QTextStream stream(&string);
	
	stream << name << "=" << value;
	
	for (QString& attribute: attributes.keys()) {
		stream << "; " << attribute << "=" << attributes[attribute];
	}
	if (secure) stream << "; " << Secure;
	if (httpOnly) stream << "; " << HttpOnly;
	
	return string;
}



bool Cookies::isEmpty() const {
	return cookies.isEmpty();
}

void Cookies::set(const Cookie& cookie) {
	cookies[cookie.getName()] = cookie;
}

bool Cookies::has(const QString& name) const {
	return cookies.contains(name);
}

Cookie Cookies::get(const QString& name) const {
	return has(name) ? cookies[name] : Cookie();
}

Cookie& Cookies::operator[](const QString& name) {
	return cookies[name];
}

const Cookie Cookies::operator[](const QString& name) const {
	return cookies[name];
}

void Cookies::clear(Cookie& cookie) {
	cookie.setValue("deleted");
	cookie.setExpires(QDateTime::fromTime_t(0));
	cookies[cookie.getName()] = cookie;
}

Cookies Cookies::fromString(const QString& cookiesString) {
	Cookies cookies;
	cookies.parse(cookiesString);
	return cookies;
}

void Cookies::parse(const QString& cookiesString) {
	QStringList parts = cookiesString.split(";");
	for (QString& part: parts) {
		if (!part.contains("=")) continue;
		QStringList nameValue = part.trimmed().split("=");
		set(Cookie(nameValue[0], nameValue[1]));
	}
}

QHash<QString, Cookie>::iterator Cookies::begin() {
	return cookies.begin();
}

QHash<QString, Cookie>::const_iterator Cookies::begin() const {
	return cookies.begin();
}

QHash<QString, Cookie>::iterator Cookies::end() {
	return cookies.end();
}

QHash<QString, Cookie>::const_iterator Cookies::end() const {
	return cookies.end();
}
