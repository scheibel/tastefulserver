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

#include <tastefulserver/HttpProtocol.h>
#include <tastefulserver/Connection.h>

#include <tastefulserver/HttpHeader.h>

namespace tastefulserver {

HttpProtocol::HttpProtocol(const RequestCallback & callback)
: m_callback(callback)
, m_parser(nullptr)
{
}

HttpProtocol::~HttpProtocol()
{
    delete m_parser;
}

void HttpProtocol::sendResponse(const HttpResponse & response)
{
    send(response.toByteArray());
}

void HttpProtocol::receive(const QByteArray & data)
{
    if (!m_parser)
    {
        m_parser = new HttpRequestParser(connection());
    }

    m_parser->addData(data);

    while (m_parser->parse())
    {
        HttpRequest request = m_parser->getRequest();

        if (!m_callback(request, *this))
        {
            return;
        }
    }
}

} // namespace tastefulserver
