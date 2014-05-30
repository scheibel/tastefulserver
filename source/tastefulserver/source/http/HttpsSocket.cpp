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

#include <tastefulserver/HttpsSocket.h>

#include <QSslSocket>

namespace tastefulserver {

HttpsSocket::HttpsSocket(HttpSocketHandler * handler, const QSslCertificate & certificate, const QSslKey & privateKey)
: HttpSocket(handler)
, m_certificate(certificate)
, m_privateKey(privateKey)
{
}


QAbstractSocket * HttpsSocket::createSocket(qintptr socketDescriptor)
{
    QSslSocket * socket = new QSslSocket();

    socket->setSocketDescriptor(socketDescriptor);

    socket->setLocalCertificate(m_certificate);
    socket->setPrivateKey(m_privateKey);

    socket->startServerEncryption();

    return socket;
}

void HttpsSocket::addConnectionInfo(HttpRequest & request)
{
    HttpSocket::addConnectionInfo(request);
    request.setHttps(true);
}


} // namespace tastefulserver
