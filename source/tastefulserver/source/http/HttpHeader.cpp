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
    : m_name(name)
    , m_value(value)
{
}

const QString & NameValuePair::getName() const
{
    return m_name;
}

const QString & NameValuePair::getValue() const
{
    return m_value;
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

    stream << m_name;
    if (!m_value.isEmpty())
    {
        stream << "=" << m_value;
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
        m_name = param;
    }
    else
    {
        m_name = param.left(pos);
        m_value = stripQuotes(param.mid(pos + 1));
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

    m_name = me.getName();
    m_value = me.getValue();
    for (int i = 1;i<parts.size();++i)
    {
        HttpHeaderElementParameter param = HttpHeaderElementParameter::fromString(parts[i].trimmed());
        m_parameters.insert(param.getName(), param);
    }
}

bool HttpHeaderElement::isSet(const QString & paramName) const
{
    return m_parameters.contains(paramName);
}

QString HttpHeaderElement::getParameter(const QString & paramName) const
{
    return m_parameters[paramName].getValue();
}

QList<HttpHeaderElementParameter> HttpHeaderElement::getParameters() const
{
    return m_parameters.values();
}

QString HttpHeaderElement::toString() const
{
    QString string;
    QTextStream stream(&string);

    stream << m_name;
    if (!m_value.isEmpty())
    {
        stream << "=" << m_value;
    }
    for (const HttpHeaderElementParameter & param : m_parameters)
    {
        stream << "; " << param.toString();
    }

    return string;
}

HttpHeaderValue::HttpHeaderValue()
    : m_modified(false)
{
}

HttpHeaderValue::HttpHeaderValue(const QString & value)
    : m_value(value)
    , m_modified(false)
{
}

HttpHeaderElement HttpHeaderValue::getElement() const
{
    parseElementsIfNecessary();

    return m_elements.first();
}

QList<HttpHeaderElement> HttpHeaderValue::getElements() const
{
    parseElementsIfNecessary();

    return m_elements;
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
        m_elements.append(newElements);
        m_modified = true;
    }
}

QString HttpHeaderValue::toString() const
{
    if (m_modified)
    {
        QString string;
        QTextStream stream(&string);

        for (int i = 0;i<m_elements.size();++i)
        {
            if (i>0)
            {
                stream << ", ";
            }
            stream << m_elements[i].toString();
        }

        return string;
    }

    return m_value;
}

void HttpHeaderValue::parseElementsIfNecessary() const
{
    if (m_elements.isEmpty())
    {
        QStringList parts = m_value.split(',');
        for (QString & part : parts)
        {
            m_elements << HttpHeaderElement::fromString(part);
        }
    }
}

HttpHeader::HttpHeader()
{
}

HttpHeader::HttpHeader(const QString & name, const QString & value)
    : m_name(name)
    , m_value(value)
{
}

QString HttpHeader::getName() const
{
    return m_name;
}

QString HttpHeader::getValue() const
{
    return m_value.toString();
}

void HttpHeader::setValue(const QString & value)
{
    m_value = value;
}

bool HttpHeader::isValid() const
{
    return !m_name.isEmpty() && m_value.isValid();
}

bool HttpHeader::merge(const HttpHeader & header)
{
    if (m_name!=header.getName())
    {
        return false;
    }
    m_value.merge(header.getElements());

    return true;
}

HttpHeaderElement HttpHeader::getElement() const
{
    return m_value.getElement();
}

QList<HttpHeaderElement> HttpHeader::getElements() const
{
    return m_value.getElements();
}

QString HttpHeader::toString() const
{
    QString string;
    QTextStream stream(&string);

    stream << m_name << ": " << m_value.toString();

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
    m_name = headerString.left(pos);
    m_value = headerString.mid(pos + 1).trimmed();
}

} // namespace tastefulserver
