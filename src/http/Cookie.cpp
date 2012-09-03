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

Cookie::Cookie(QString name, QString value) : name(name), value(value), secure(false), httpOnly(false) {
}

QString Cookie::getName() {
	return name;
}

QString Cookie::getValue() {
	return value;
}

void Cookie::setValue(QString value) {
	this->value = value;
}

void Cookie::setMaxAge(unsigned seconds) {
	attributes[MaxAge] = QString::number(seconds);
}

void Cookie::setComment(QString comment) {
	attributes[Comment] = comment;
}
	
void Cookie::setDomain(QString domain) {
	attributes[Domain] = domain;
}
	
void Cookie::setPath(QString path) {
	attributes[Path] = path;
}
	
void Cookie::setVersion(QString version) {
	attributes[Version] = version;
}
	
void Cookie::setExpires(const QDateTime& dateTime) {
	setExpires(http::dateString(dateTime));
}
	
void Cookie::setExpires(QString expires) {
	attributes[Expires] = expires;
}
	
void Cookie::setSecure(bool secure) {
	this->secure = secure;
}
	
void Cookie::setHttpOnly(bool httpOnly) {
	this->httpOnly = httpOnly;
}

QString Cookie::toString() {
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



bool Cookies::isEmpty() {
	return cookies.isEmpty();
}

void Cookies::set(Cookie cookie) {
	cookies[cookie.getName()] = cookie;
}

bool Cookies::has(const QString& name) {
	return cookies.contains(name);
}

Cookie Cookies::get(const QString& name) {
	return has(name) ? cookies[name] : Cookie();
}

Cookie& Cookies::operator[](const QString& name) {
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

QHash<QString, Cookie>::iterator Cookies::end() {
	return cookies.end();
}
