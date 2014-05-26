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

#include <tastefulserver/HttpHandler.h>
#include <tastefulserver/Connection.h>

#include <QStringList>

#include <tastefulserver/HttpHeader.h>
#include <tastefulserver/websockets.h>
#include <tastefulserver/WebsocketHandler.h>

namespace tastefulserver {

HttpHandler::HttpHandler(const RequestCallback & callback)
: m_callback(callback)
, m_buffer(ByteArrayStream::forLinebreak(http::Linebreak))
{
    m_badRequestCallback = [](const HttpRequest & request) {
            HttpResponse response(request);

            response.setStatusCode(http::BadRequest);

            return response;
        };

    m_state = READ_REQUEST_LINE;
}

HttpHandler::HttpHandler(const RequestCallback & callback, const RequestCallback & badRequestCallback)
: m_callback(callback)
, m_badRequestCallback(badRequestCallback)
, m_hasBadRequestCallback(true)
, m_buffer(ByteArrayStream::forLinebreak(http::Linebreak))
{
    m_state = READ_REQUEST_LINE;
}

void HttpHandler::receive(const QByteArray & data)
{
    m_buffer.append(data);

    bool continueReading = true;

    while (continueReading)
    {
        switch (m_state)
        {
            case READ_REQUEST_LINE:
                continueReading = readRequestLine();
                break;
            case READ_HEADER:
                continueReading = readHeader();
                break;
            case READ_CONTENT:
                continueReading = readContent();
                break;
            case HANDLE_REQUEST:
                continueReading = handleRequest();
                break;
            case HANDLE_ERROR:
                continueReading = handleError();
                break;
        }
    }
}

void HttpHandler::setBadRequestCallback(const RequestCallback & badRequestCallback)
{
    m_badRequestCallback = badRequestCallback;
    m_hasBadRequestCallback = true;
}

void HttpHandler::uninstallBadRequestCallback()
{
    m_badRequestCallback = [](const HttpRequest & request) {
            HttpResponse response(request);

            response.setStatusCode(http::BadRequest);

            return response;
        };

    m_hasBadRequestCallback = false;
}

bool HttpHandler::readRequestLine()
{
    if (!m_buffer.canReadLine())
    {
        return false;
    }

    QString line = m_buffer.readLine();

    if (line.isEmpty())
    {
        return true;
    }

    QStringList parts = line.split(' ');
    if (parts.size()<3)
    {
        m_state = HANDLE_ERROR;

        return true;
    }
    HttpMethod method = HttpMethod::fromString(parts[0]);
    HttpVersion httpVersion = HttpVersion::fromString(parts[2]);

    if (method.isInvalid() || httpVersion.isInvalid())
    {
        m_state = HANDLE_ERROR;

        return true;
    }

    QString requestUri = parts[1];

    m_request = HttpRequest(method, requestUri, httpVersion, connection()->isSslConnection());
    m_request.setAddress(connection()->socket().peerAddress());
    m_request.setPort(connection()->socket().peerPort());

    m_state = READ_HEADER;

    return true;
}

bool HttpHandler::readHeader()
{
    if (!m_buffer.canReadLine())
    {
        return false;
    }

    QString line = m_buffer.readLine();
    if (line.isEmpty())
    {
        m_state = m_request.hasHeader(http::ContentLength) ? READ_CONTENT : HANDLE_REQUEST;

        return true;
    }
    else
    {
        int pos = line.indexOf(": ");
        if (pos<0)
        {
            m_request.markBad();

            return true;
        }

        QString fieldName = line.left(pos);
        QString fieldValue = line.mid(pos + 2);
        HttpHeader header(fieldName, fieldValue);
        m_request.parseHeader(header);
    }

    return true;
}

bool HttpHandler::readContent()
{
    int length = m_request.getContentLength();

    if (m_buffer.availableBytes()<length)
    {
        return false;
    }
    QByteArray content = m_buffer.read(length);
    m_request.parseContent(content);
    m_state = HANDLE_REQUEST;

    return true;
}

bool HttpHandler::handleRequest()
{
    HttpResponse response = m_hasBadRequestCallback && m_request.isBad() ? m_badRequestCallback(m_request) : m_callback(m_request);

    bool doUpgrade = false;
    if (m_request.hasHeader(http::Upgrade))
    {
        response = websocketHandshake(m_request);
        doUpgrade = true;
    }

    send(response.toByteArray());
    m_buffer.flush();

    if (doUpgrade)
    {
        connection()->switchProtocol(new WebsocketHandler());

        return false;
    }

    m_state = READ_REQUEST_LINE;

    if (!response.isKeepAlive())
    {
        disconnect();
    }

    return false;
}

bool HttpHandler::handleError()
{
    m_request.markBad();

    HttpResponse response;
    response.setStatusCode(http::BadRequest);
    send(response.toByteArray());

    m_state = READ_REQUEST_LINE;

    return true;
}

} // namespace tastefulserver
