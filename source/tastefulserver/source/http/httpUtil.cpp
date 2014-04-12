/**
 * (C) LGPL-3
 *
 * Tasteful Server <https://github.com/scheibel/tasteful-server>
 *
 * Copyright: 2012 Lux, Scheibel
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

#include <tastefulserver/httpUtil.h>
#include <tastefulserver/httpStatusCodes.h>

#include <QHash>

namespace tastefulserver {

QHash<unsigned, QString> makeReasonHash()
{
    QHash<unsigned, QString> hash;

    hash.reserve(50);

    hash.insert(http::Continue, "Continue");
    hash.insert(http::SwitchingProtocols, "Switching Protocols");
    hash.insert(http::OK, "OK");
    hash.insert(http::Created, "Created");
    hash.insert(http::Accepted, "Accepted");
    hash.insert(http::NonAuthoritativeInformation, "Non-Authoritative Information");
    hash.insert(http::NoContent, "No Content");
    hash.insert(http::ResetContent, "Reset Content");
    hash.insert(http::PartialContent, "Partial Content");

    hash.insert(http::MultipleChoices, "Multiple Choices");
    hash.insert(http::MovedPermanently, "Moved Permanently");
    hash.insert(http::Found, "Found");
    hash.insert(http::SeeOther, "See Other");
    hash.insert(http::NotModified, "Not Modified");
    hash.insert(http::UseProxy, "Use Proxy");
    hash.insert(http::TemporaryRedirect, "Temporary Redirect");

    hash.insert(http::BadRequest, "Bad Request");
    hash.insert(http::Unauthorized, "Unauthorized");
    hash.insert(http::PaymentRequired, "Payment Required");
    hash.insert(http::Forbidden, "Forbidden");
    hash.insert(http::NotFound, "Not Found");
    hash.insert(http::MethodNotAllowed, "Method Not Allowed");
    hash.insert(http::NotAcceptable, "Not Acceptable");
    hash.insert(http::ProxyAuthenticationRequired, "Proxy Authentication Required");
    hash.insert(http::RequestTimeout, "Request Time-out");
    hash.insert(http::Conflict, "Conflict");
    hash.insert(http::Gone, "Gone");
    hash.insert(http::LengthRequired, "Length Required");
    hash.insert(http::PreconditionFailed, "PreconditionFailed");
    hash.insert(http::RequestEntityTooLarge, "Request Entity Too Large");
    hash.insert(http::RequestURITooLarge, "Request-URI Too Large");
    hash.insert(http::UnsupportedMediaType, "Unsupported Media Type");
    hash.insert(http::RequestedRangeNotSatisfiable, "Requested range not satisfiable");
    hash.insert(http::ExpectationFailed, "Expectation Failed");

    hash.insert(http::InternalServerError, "Internal Server Error");
    hash.insert(http::NotImplemented, "Not Implemented");
    hash.insert(http::BadGateway, "Bad Gateway");
    hash.insert(http::ServiceUnavailable, "Service Unavailable");
    hash.insert(http::InternalServerError, "Gateway Time-out");
    hash.insert(http::HTTPVersionNotSupported, "HTTP Version not supported");

    hash.squeeze();

    return hash;
}

namespace http {

QString reason(const unsigned statusCode)
{
    static QHash<unsigned, QString> hash = makeReasonHash();

    return hash[statusCode];
}

QString dateString(const QDateTime & dateTime)
{
    return dateTime.toUTC().toString("ddd, dd MMM yyyy hh:mm:ss 'GMT'");
}

} // namespace http

} // namespace tastefulserver
