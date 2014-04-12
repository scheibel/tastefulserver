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

#include <tastefulserver/HttpMessage.h>
#include <tastefulserver/HttpHeader.h>

HttpVersion::HttpVersion() : _major(0), _minor(0) {
}

HttpVersion::HttpVersion(unsigned _major, unsigned _minor) : _major(_major), _minor(_minor) {
}

bool HttpVersion::isInvalid() const {
	return _major==0 && _minor==0;
}

unsigned HttpVersion::getMajor() const {
	return _major;
}

unsigned HttpVersion::getMinor() const {
	return _minor;
}

bool HttpVersion::operator<(const HttpVersion& httpVersion) const {
	return _major<httpVersion._major && _minor<httpVersion._minor;
}

bool HttpVersion::operator<=(const HttpVersion& httpVersion) const {
	return _major<=httpVersion._major && _minor<=httpVersion._minor;
}

bool HttpVersion::operator>(const HttpVersion& httpVersion) const {
	return _major>httpVersion._major && _minor>httpVersion._minor;
}

bool HttpVersion::operator>=(const HttpVersion& httpVersion) const {
	return _major>=httpVersion._major && _minor>=httpVersion._minor;
}

bool HttpVersion::operator==(const HttpVersion& httpVersion) const {
	return _major==httpVersion._major && _minor==httpVersion._minor;
}

bool HttpVersion::operator!=(const HttpVersion& httpVersion) const {
	return _major!=httpVersion._major || _minor!=httpVersion._minor;
}

QString HttpVersion::toString() const {
	return QString("HTTP/%1.%2").arg(_major).arg(_minor);
}

HttpVersion HttpVersion::fromString(const QString& string) {
	if (!string.startsWith("HTTP/")) return HttpVersion();
	QString version = string.mid(5);
	int pos = version.indexOf('.');
	if (pos<0) return HttpVersion();
	unsigned _major = version.left(pos).toUInt();
	unsigned _minor = version.mid(pos+1).toUInt();
	return HttpVersion(_major, _minor);
}
