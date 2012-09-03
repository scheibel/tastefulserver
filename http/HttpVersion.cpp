#include <HttpMessage>
#include <HttpHeader>

HttpVersion::HttpVersion() : _major(0), _minor(0) {
}

HttpVersion::HttpVersion(unsigned _major, unsigned _minor) : _major(_major), _minor(_minor) {
}

bool HttpVersion::isInvalid() {
	return _major==0 && _minor==0;
}

unsigned HttpVersion::getMajor() {
	return _major;
}

unsigned HttpVersion::getMinor() {
	return _minor;
}

bool HttpVersion::operator<(const HttpVersion& httpVersion) {
	return _major<httpVersion._major && _minor<httpVersion._minor;
}

QString HttpVersion::toString() {
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
