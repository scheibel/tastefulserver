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

#pragma once

#include <functional>

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/TcpServer.h>
#include <tastefulserver/Protocol.h>
#include <tastefulserver/HttpHandler.h>
#include <tastefulserver/WebsocketHandler.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpServer : public TcpServer, public HttpHandler, public WebsocketHandler
{
public:
    typedef std::function<HttpResponse(const HttpRequest &)> RequestCallback;

    HttpServer(const RequestCallback & callback);
    ~HttpServer();

protected:
    RequestCallback m_callback;
    TcpSocketFactory * m_socketFactory;

    virtual SocketFactory * getSocketFactory() override;
    virtual Protocol * createProtocol() override;

    // --- handling ---

    virtual void handleRequest(HttpProtocol * protocol, const HttpRequest & request) override;
    virtual void handleBadRequest(HttpProtocol * protocol) override;

    virtual void handleFrame(WebsocketProtocol * protocol, const WebsocketFrame & frame) override;
    virtual void handleBadFrame(WebsocketProtocol * protocol) override;
};

} // namespace tastefulserver
