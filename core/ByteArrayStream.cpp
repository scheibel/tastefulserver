#include <internal/ByteArrayStream>

using namespace internal;

ByteArrayStream::ByteArrayStream(QByteArray bytes, QString linebreak) : buffer(bytes), linebreak(linebreak), pos(0) {
}

ByteArrayStream ByteArrayStream::forLinebreak(QString linebreak) {
	return ByteArrayStream(QByteArray(), linebreak);
}

void ByteArrayStream::append(QByteArray bytes) {
	buffer.append(bytes);
}

void ByteArrayStream::flush() {
	buffer = buffer.remove(0,pos);
	pos = 0;
}

bool ByteArrayStream::canReadLine() {
	return buffer.indexOf(linebreak, pos)>=0;
}

int ByteArrayStream::availableBytes() {
	return buffer.size()-pos;
}

QByteArray ByteArrayStream::read(unsigned length) {
	if (availableBytes()<length) {
		pos = buffer.size();
		return buffer.mid(pos);
	}
	QByteArray bytes = buffer.mid(pos, length);
	pos+=length;
	return bytes;
}

void ByteArrayStream::skip(unsigned length) {
	if (availableBytes()<length) {
		pos = buffer.size();
		return;
	}
	pos+=length;
}

QString ByteArrayStream::readLine() {
	int p = buffer.indexOf(linebreak, pos);
	if (p<0) return readAll();
	QString line = QString(buffer.mid(pos, p-pos));
	pos = p+linebreak.size();
	return line;
}

bool ByteArrayStream::canReadUpTo(QString delimiter) {
	return buffer.indexOf(delimiter, pos)>=0;
}

bool ByteArrayStream::canReadUpTo(QChar delimiter) {
	return buffer.indexOf(delimiter, pos)>=0;
}

void ByteArrayStream::skipUpTo(QString delimiter) {
	int p = buffer.indexOf(delimiter, pos);
	if (p<0) {
		skipAll();
	} else {
		pos = p;
	}
}

void ByteArrayStream::skipUpTo(QChar delimiter) {
	int p = buffer.indexOf(delimiter, pos);
	if (p<0) {
		skipAll();
	} else {
		pos = p;
	}
}

void ByteArrayStream::skipBehind(QString delimiter) {
	skipUpTo(delimiter);
	skip(delimiter.size());
}

void ByteArrayStream::skipBehind(QChar delimiter) {
	skipUpTo(delimiter);
	skip(1);
}

QByteArray ByteArrayStream::readUpTo(QString delimiter, bool skipDelimiter) {
	int p = buffer.indexOf(delimiter, pos);
	if (p<0) return readAll();
	QByteArray part = buffer.mid(pos, p-pos);
	pos = p;
	if (skipDelimiter) skip(delimiter.size());
	return part;	
}

QByteArray ByteArrayStream::readUpTo(QChar delimiter, bool skipDelimiter) {
	int p = buffer.indexOf(delimiter, pos);
	if (p<0) return readAll();
	QByteArray part = buffer.mid(pos, p-pos);
	pos = p;
	if (skipDelimiter) skip(1);
	return part;	
}

void ByteArrayStream::skipAll() {
	pos = buffer.size();
}

QByteArray ByteArrayStream::readAll() {
	QByteArray bytes = buffer.mid(pos);
	pos = buffer.size();
	return bytes;
}

bool ByteArrayStream::atEnd() {
	return pos == buffer.size();
}
