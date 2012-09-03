#include <ContentType>

const QString ContentType::Charset = "charset";
const QString ContentType::Boundary = "boundary";

const QString ContentType::Multipart = "multipart";
const QString ContentType::Application = "application";
	
const QString ContentType::FormData = "form-data";
const QString ContentType::XWWWFormUrlEncoded = "x-www-form-urlencoded";

ContentType::ContentType() {
}

ContentType::ContentType(QString type, QString subtype) : type(type), subtype(subtype) {
	name = type+'/'+subtype;
}

bool ContentType::is(QString type, QString subtype) {
	return this->type==type && this->subtype==subtype;
}

void ContentType::setType(const QString& type) {
	this->type = type;
}

void ContentType::setSubype(const QString& subtype) {
	this->subtype = subtype;
}

void ContentType::setTypeAndSubtype(const QString& typeAndSubype) {
	int pos = typeAndSubype.indexOf('/');
	if (pos<0) {
		type = name;
		subtype = QString();
	} else {
		type = name.left(pos);
		subtype = name.mid(pos+1);
	}
}

ContentType ContentType::fromString(const QString& value) {
	ContentType contentType;
	contentType.parse(value);
	return contentType;
}

void ContentType::parse(const QString& value) {
	HttpHeaderElement::parse(value);
	setTypeAndSubtype(name);
}

QString ContentType::getType() {
	return type;
}

QString ContentType::getSubtype() {
	return subtype;
}

QString ContentType::getCharset() {
	return getParameter(Charset);
}

QString ContentType::getBoundary() {
	return getParameter(Boundary);
}

bool ContentType::isMultiPart() {
	return type==Multipart;
}