#include <HttpHeader>
#include <QStringList>
#include <QTextStream>

NameValuePair::NameValuePair() {
}

NameValuePair::NameValuePair(QString name, QString value) : name(name), value(value) {
}

QString NameValuePair::getName() {
	return name;
}

QString NameValuePair::getValue() {
	return value;
}

HttpHeaderElementParameter::HttpHeaderElementParameter() : NameValuePair() {
}

HttpHeaderElementParameter::HttpHeaderElementParameter(QString name, QString value) : NameValuePair(name, value) {
}

QString HttpHeaderElementParameter::toString() {
	QString string;
	QTextStream stream(&string);
	stream << name;
	if (!value.isEmpty()) stream << "=" << value;
	return string;	
}

HttpHeaderElementParameter HttpHeaderElementParameter::fromString(const QString& param) {
	HttpHeaderElementParameter parameter;
	parameter.parse(param);
	return parameter;
}

void HttpHeaderElementParameter::parse(const QString& param) {
	int pos = param.indexOf('=');
	if (pos<0) {
		name = param;
	} else {
		name = param.left(pos);
		value = stripQuotes(param.mid(pos+1));
	}
}

QString HttpHeaderElementParameter::stripQuotes(const QString& string) {
	if (string.startsWith('"') && string.endsWith('"')) return string.mid(1, string.size()-2);
	return string;
}

HttpHeaderElement::HttpHeaderElement() : NameValuePair() {
}

HttpHeaderElement::HttpHeaderElement(QString name, QString value) : NameValuePair(name, value) {
}

HttpHeaderElement HttpHeaderElement::fromString(const QString& elementString) {
	HttpHeaderElement element;
	element.parse(elementString);
	return element;
}

void HttpHeaderElement::parse(const QString& elementString) {
	QStringList parts = elementString.split(';');
	HttpHeaderElementParameter me = HttpHeaderElementParameter::fromString(parts[0].trimmed());
	name = me.getName();
	value = me.getValue();
	for (int i=1; i<parts.size(); ++i) {
		HttpHeaderElementParameter param = HttpHeaderElementParameter::fromString(parts[i].trimmed());
		parameters.insert(param.getName(), param);
	}
}

bool HttpHeaderElement::isSet(const QString& paramName) {
	return parameters.contains(paramName);
}

QString HttpHeaderElement::getParameter(const QString& paramName) {
	return parameters[paramName].getValue();
}

QList<HttpHeaderElementParameter> HttpHeaderElement::getParameters() {
	return parameters.values();
}

QString HttpHeaderElement::toString() {
	QString string;
	QTextStream stream(&string);
	stream << name;
	if (!value.isEmpty()) stream << "=" << value;
	for (HttpHeaderElementParameter& param: parameters) {
		stream << "; " << param.toString();
	}
	return string;
}

HttpHeaderValue::HttpHeaderValue() : modified(false) {
}

HttpHeaderValue::HttpHeaderValue(QString value) : value(value), modified(false) {
}

HttpHeaderElement HttpHeaderValue::getElement() {
	parseElementsIfNecessary();
	return elements.first();	
}

QList<HttpHeaderElement> HttpHeaderValue::getElements() {
	parseElementsIfNecessary();
	return elements;	
}

bool HttpHeaderValue::isValid() {
	return !toString().isEmpty();
}

bool HttpHeaderValue::merge(QList<HttpHeaderElement> newElements)  {
	parseElementsIfNecessary();
	if (!newElements.isEmpty()) {
		for (HttpHeaderElement& element: newElements) {
			elements << element;
		}
		modified = true;
	}
	return true;	
}

QString HttpHeaderValue::toString() {
	if (modified) {
		QString string;
		QTextStream stream(&string);
		
		for (int i=0; i<elements.size(); ++i) {
			if (i>0) stream << ", ";
			stream << elements[i].toString();
		}
		return string;
	}
	return value;	
}

void HttpHeaderValue::parseElementsIfNecessary() {
	if (elements.isEmpty()) {
		QStringList parts = value.split(',');
		for (QString& part: parts) {
			elements << HttpHeaderElement::fromString(part);
		}
	}	
}

HttpHeader::HttpHeader() {
}

HttpHeader::HttpHeader(QString name, QString value) : name(name), value(value) {
}

QString HttpHeader::getName() {
	return name;
}

QString HttpHeader::getValue() {
	return value.toString();
}

void HttpHeader::setValue(const QString& value) {
	this->value = value;
}

bool HttpHeader::isValid() {
	return !name.isEmpty() && value.isValid();
}

bool HttpHeader::merge(HttpHeader header) {
	if (name!=header.getName()) return false;
	return value.merge(header.getElements());
}

HttpHeaderElement HttpHeader::getElement() {
	return value.getElement();
}

QList<HttpHeaderElement> HttpHeader::getElements() {
	return value.getElements();
}

QString HttpHeader::toString() {
	QString string;
	QTextStream stream(&string);
	stream << name << ": " << value.toString();
	return string;
}

HttpHeader HttpHeader::forName(const QString& name) {
	return HttpHeader(name, QString());
}

HttpHeader HttpHeader::fromString(const QString& headerString) {
	HttpHeader header;
	header.parse(headerString);
	return header;
}

void HttpHeader::parse(const QString& headerString) {
	int pos = headerString.indexOf(':');
	if (pos<0) return;
	name = headerString.left(pos);
	value = headerString.mid(pos+1).trimmed();
}
