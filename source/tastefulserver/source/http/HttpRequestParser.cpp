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

#include <tastefulserver/HttpRequestParser.h>

#include <QStringList>

#include <tastefulserver/HttpHeader.h>

namespace tastefulserver {

HttpRequestParser::HttpRequestParser(const Connection * connection)
: m_connection(connection)
, m_byteStream(ByteArrayStream::forLinebreak(http::Linebreak))
, m_state(State::ParseRequestLine)
{
}

void HttpRequestParser::addData(const QByteArray & data)
{
    m_byteStream.append(data);
}

bool HttpRequestParser::parse()
{
    bool continueReading = true;

    while (continueReading)
    {
        switch (m_state)
        {
            case State::ParseRequestLine:
                continueReading = parseRequestLine();
                break;
            case State::ParseHeaderLine:
                continueReading = parseHeaderLine();
                break;
            case State::ParseContent:
                continueReading = parseContent();
                break;
            case State::FinishRequest:
                continueReading = finishRequest();
                break;
            case State::HandleError:
                continueReading = handleError();
                break;
        }
    }

    return hasReadyRequest();
}

bool HttpRequestParser::hasReadyRequest()
{
    return !m_readyRequests.empty();
}

HttpRequest HttpRequestParser::getRequest()
{
    HttpRequest request = m_readyRequests.front();
    m_readyRequests.pop_front();

    return request;
}

bool HttpRequestParser::parseRequestLine()
{
    if (!m_byteStream.canReadLine())
    {
        return false;
    }

    QString line = m_byteStream.readLine();

    if (line.isEmpty())
    {
        return true;
    }

    QStringList parts = line.split(' ');
    if (parts.size()<3)
    {
        m_state = State::HandleError;

        return true;
    }

    HttpMethod method = HttpMethod::fromString(parts[0]);
    HttpVersion httpVersion = HttpVersion::fromString(parts[2]);

    if (method.isInvalid() || httpVersion.isInvalid())
    {
        m_state = State::HandleError;

        return true;
    }

    QString requestUri = parts[1];

    m_currentRequest = HttpRequest(method, requestUri, httpVersion, m_connection->isSslConnection());
    m_currentRequest.setAddress(m_connection->socket().peerAddress());
    m_currentRequest.setPort(m_connection->socket().peerPort());

    m_state = State::ParseHeaderLine;

    return true;
}

bool HttpRequestParser::parseHeaderLine()
{
    if (!m_byteStream.canReadLine())
    {
        return false;
    }

    QString line = m_byteStream.readLine();
    if (line.isEmpty())
    {
        m_state = m_currentRequest.hasHeader(http::ContentLength) ? State::ParseContent : State::FinishRequest;

        return true;
    }
    else
    {
        int pos = line.indexOf(": ");
        if (pos<0)
        {
            m_currentRequest.markBad();

            return true;
        }

        QString fieldName = line.left(pos);
        QString fieldValue = line.mid(pos + 2);
        HttpHeader header(fieldName, fieldValue);
        m_currentRequest.parseHeader(header);
    }

    return true;
}

bool HttpRequestParser::parseContent()
{
    int length = m_currentRequest.getContentLength();

    if (m_byteStream.availableBytes()<length)
    {
        return false;
    }
    QByteArray content = m_byteStream.read(length);
    m_currentRequest.parseContent(content);
    m_state = State::FinishRequest;

    return true;
}

bool HttpRequestParser::finishRequest()
{
    pushRequest();

    m_state = State::ParseRequestLine;

    m_byteStream.flush();

    return false;
}

bool HttpRequestParser::handleError()
{
    m_currentRequest.markBad();

    pushRequest();

    m_state = State::ParseRequestLine;

    return true;
}

void HttpRequestParser::pushRequest()
{
    m_readyRequests.push_back(m_currentRequest);
}

} // namespace tastefulserver
