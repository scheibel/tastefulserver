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

HttpRequestParser::HttpRequestParser()
: m_byteStream(ByteStream::forLinebreak(http::Linebreak))
, m_state(ParseState::RequestLine)
, m_interruptedState(ParseState::RequestLine)
{
}

HttpRequestParser::~HttpRequestParser()
{
}

void HttpRequestParser::addData(const QByteArray & data)
{
    m_byteStream.flush();
    m_byteStream.append(data);

    parse();
}

bool HttpRequestParser::hasReadyRequests() const
{
    return !m_readyRequests.empty();
}

HttpRequest HttpRequestParser::popReadyRequest()
{
    HttpRequest request = m_readyRequests.front();
    m_readyRequests.pop_front();

    return request;
}

void HttpRequestParser::pushRequest()
{
    m_readyRequests.push_back(m_currentRequest);
}

void HttpRequestParser::parse()
{
    while (true)
    {
        ParseState nextState = dispatch(m_state);

        if (nextState == ParseState::Interrupted)
        {
            m_interruptedState = m_state;
            break;
        }

        m_state = nextState;
    }
}

HttpRequestParser::ParseState HttpRequestParser::dispatch(ParseState state)
{
    switch (state)
    {
        case ParseState::RequestLine:
            return parseRequestLine();
        case ParseState::HeaderLine:
            return parseHeaderLine();
        case ParseState::Content:
            return parseContent();
        case ParseState::Finish:
            return finishRequest();
        case ParseState::Error:
            return handleError();
        case ParseState::Interrupted:
            return m_interruptedState;
        default: // for compiler, cannot happen
            return ParseState::Error;
    }
}

HttpRequestParser::ParseState HttpRequestParser::parseRequestLine()
{
    if (!m_byteStream.canReadLine())
    {
        return ParseState::Interrupted;
    }

    QString line = m_byteStream.readLine();

    if (line.isEmpty())
    {
        return ParseState::RequestLine;
    }

    QStringList parts = line.split(' ');
    if (parts.size()<3)
    {
        return ParseState::Error;
    }

    HttpMethod method = HttpMethod::fromString(parts[0]);
    HttpVersion httpVersion = HttpVersion::fromString(parts[2]);

    if (method.isInvalid() || httpVersion.isInvalid())
    {
        return ParseState::Error;
    }

    QString requestUri = parts[1];

    m_currentRequest = HttpRequest(method, requestUri, httpVersion);

    return ParseState::HeaderLine;
}

HttpRequestParser::ParseState HttpRequestParser::parseHeaderLine()
{
    if (!m_byteStream.canReadLine())
    {
        return ParseState::Interrupted;
    }

    QString line = m_byteStream.readLine();

    if (line.isEmpty())
    {
        return m_currentRequest.hasHeader(http::ContentLength) ? ParseState::Content : ParseState::Finish;
    }

    int pos = line.indexOf(": ");
    if (pos<0)
    {
        return ParseState::Error;
    }

    QString fieldName = line.left(pos);
    QString fieldValue = line.mid(pos + 2);

    m_currentRequest.addHeader(HttpHeader(fieldName, fieldValue));

    return ParseState::HeaderLine;
}

HttpRequestParser::ParseState HttpRequestParser::parseContent()
{
    int length = m_currentRequest.getContentLength();

    if (m_byteStream.availableBytes()<length)
    {
        return ParseState::Interrupted;
    }

    m_currentRequest.setContent(m_byteStream.read(length));

    return ParseState::Finish;
}

HttpRequestParser::ParseState HttpRequestParser::finishRequest()
{
    m_currentRequest.finalize();

    pushRequest();

    return ParseState::RequestLine;
}

HttpRequestParser::ParseState HttpRequestParser::handleError()
{
    emit(badRequest());

    return ParseState::RequestLine;
}

} // namespace tastefulserver
