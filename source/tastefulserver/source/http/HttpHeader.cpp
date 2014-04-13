/**
 * (C) LGPL-3
 *
 * Tasteful Server <https://github.com/scheibel/tasteful-server>
 *
 * Copyright: 2012-2014 Lux, Scheibel
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

#include <tastefulserver/HttpHeader.h>

#include <QStringList>
#include <QTextStream>

namespace tastefulserver {

NameValuePair::NameValuePair()
{
}

NameValuePair::NameValuePair(const QString & name, const QString & value)
    : name(name)
    , value(value)
{
}

QString NameValuePair::getName() const
{
    return name;
}

QString NameValuePair::getValue() const
{
    return value;
}

HttpHeaderElementParameter::HttpHeaderElementParameter()
{
}

HttpHeaderElementParameter::HttpHeaderElementParameter(const QString & name, const QString & value)
    : NameValuePair(name, value)
{
}

QString HttpHeaderElementParameter::toString() const
{
    QString string;
    QTextStream stream(&string);

    stream << name;
    if (!value.isEmpty())
    {
        stream << "=" << value;
    }

    return string;
}

HttpHeaderElementParameter HttpHeaderElementParameter::fromString(const QString & param)
{
    HttpHeaderElementParameter parameter;

    parameter.parse(param);

    return parameter;
}

void HttpHeaderElementParameter::parse(const QString & param)
{
    int pos = param.indexOf('=');

    if (pos<0)
    {
        name = param;
    }
    else
    {
        name = param.left(pos);
        value = stripQuotes(param.mid(pos + 1));
    }
}

QString HttpHeaderElementParameter::stripQuotes(const QString & string)
{
    if (string.startsWith('"') && string.endsWith('"'))
    {
        return string.mid(1, string.size() - 2);
    }

    return string;
}

HttpHeaderElement::HttpHeaderElement()
    : NameValuePair()
{
}

HttpHeaderElement::HttpHeaderElement(const QString & name, const QString & value)
    : NameValuePair(name, value)
{
}

HttpHeaderElement HttpHeaderElement::fromString(const QString & elementString)
{
    HttpHeaderElement element;

    element.parse(elementString);

    return element;
}

void HttpHeaderElement::parse(const QString & elementString)
{
    QStringList parts = elementString.split(';');
    HttpHeaderElementParameter me = HttpHeaderElementParameter::fromString(parts[0].trimmed());

    name = me.getName();
    value = me.getValue();
    for (int i = 1;i<parts.size();++i)
    {
        HttpHeaderElementParameter param = HttpHeaderElementParameter::fromString(parts[i].trimmed());
        parameters.insert(param.getName(), param);
    }
}

bool HttpHeaderElement::isSet(const QString & paramName) const
{
    return parameters.contains(paramName);
}

QString HttpHeaderElement::getParameter(const QString & paramName) const
{
    return parameters[paramName].getValue();
}

QList<HttpHeaderElementParameter> HttpHeaderElement::getParameters() const
{
    return parameters.values();
}

QString HttpHeaderElement::toString() const
{
    QString string;
    QTextStream stream(&string);

    stream << name;
    if (!value.isEmpty())
    {
        stream << "=" << value;
    }
    for (const HttpHeaderElementParameter & param : parameters)
    {
        stream << "; " << param.toString();
    }

    return string;
}

HttpHeaderValue::HttpHeaderValue()
    : modified(false)
{
}

HttpHeaderValue::HttpHeaderValue(const QString & value)
    : value(value)
    , modified(false)
{
}

HttpHeaderElement HttpHeaderValue::getElement() const
{
    parseElementsIfNecessary();

    return elements.first();
}

QList<HttpHeaderElement> HttpHeaderValue::getElements() const
{
    parseElementsIfNecessary();

    return elements;
}

bool HttpHeaderValue::isValid() const
{
    return !toString().isEmpty();
}

void HttpHeaderValue::merge(const QList<HttpHeaderElement> & newElements)
{
    parseElementsIfNecessary();
    if (!newElements.isEmpty())
    {
        elements.append(newElements);
        modified = true;
    }
}

QString HttpHeaderValue::toString() const
{
    if (modified)
    {
        QString string;
        QTextStream stream(&string);

        for (int i = 0;i<elements.size();++i)
        {
            if (i>0)
            {
                stream << ", ";
            }
            stream << elements[i].toString();
        }

        return string;
    }

    return value;
}

void HttpHeaderValue::parseElementsIfNecessary() const
{
    if (elements.isEmpty())
    {
        QStringList parts = value.split(',');
        for (QString & part : parts)
        {
            elements << HttpHeaderElement::fromString(part);
        }
    }
}

HttpHeader::HttpHeader()
{
}

HttpHeader::HttpHeader(const QString & name, const QString & value)
    : name(name)
    , value(value)
{
}

QString HttpHeader::getName() const
{
    return name;
}

QString HttpHeader::getValue() const
{
    return value.toString();
}

void HttpHeader::setValue(const QString & value)
{
    this->value = value;
}

bool HttpHeader::isValid() const
{
    return !name.isEmpty() && value.isValid();
}

bool HttpHeader::merge(const HttpHeader & header)
{
    if (name!=header.getName())
    {
        return false;
    }
    value.merge(header.getElements());

    return true;
}

HttpHeaderElement HttpHeader::getElement() const
{
    return value.getElement();
}

QList<HttpHeaderElement> HttpHeader::getElements() const
{
    return value.getElements();
}

QString HttpHeader::toString() const
{
    QString string;
    QTextStream stream(&string);

    stream << name << ": " << value.toString();

    return string;
}

HttpHeader HttpHeader::forName(const QString & name)
{
    return HttpHeader(name, QString());
}

HttpHeader HttpHeader::fromString(const QString & headerString)
{
    HttpHeader header;

    header.parse(headerString);

    return header;
}

void HttpHeader::parse(const QString & headerString)
{
    int pos = headerString.indexOf(':');

    if (pos<0)
    {
        return;
    }
    name = headerString.left(pos);
    value = headerString.mid(pos + 1).trimmed();
}

} // namespace tastefulserver
