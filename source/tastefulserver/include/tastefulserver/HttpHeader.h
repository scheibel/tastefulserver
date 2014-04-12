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

#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <QString>
#include <QList>
#include <QHash>

namespace http {
	typedef const QString HeaderName;
	
	// general headers
	HeaderName CacheControl = "Cache-Control";
	HeaderName Connection = "Connection";
	HeaderName Date = "Date";
	HeaderName Pragma = "Pragma";
	HeaderName Trailer = "Trailer";
	HeaderName TransferEncoding = "Transfer-Encoding";
	HeaderName Upgrade = "Upgrade";
	HeaderName Via = "Via";
	HeaderName warning = "Warning";
	
	// entity headers
	HeaderName Allow = "Allow";
	HeaderName ContentEncoding = "Content-Encoding";
	HeaderName ContentLanguage = "Content-Language ";
	HeaderName ContentLength = "Content-Length";
	HeaderName ContentLocation = "Content-Location ";
	HeaderName ContentMD5 = "Content-MD5";
	HeaderName ContentRange = "Content-Range";
	HeaderName ContentType = "Content-Type";
	HeaderName Expires = "Expires";
	HeaderName LastModified = "Last-Modified";
	
	// multipart headers
	HeaderName ContentDisposition = "Content-Disposition";
	
	// request headers
	HeaderName Accept = "Accept";
	HeaderName AcceptCharset = "Accept-Charset";
	HeaderName AcceptEncoding = "Accept-Encoding";
	HeaderName AcceptLanguage = "Accept-Language";
	HeaderName Authorization = "Authorization";
	HeaderName Expect = "Expect";
	HeaderName From = "From";
	HeaderName Host = "Host";
	HeaderName IfMatch = "If-Match";
	HeaderName IfModifiedSince = "If-Modified-Since";
	HeaderName IfNoneMatch = "If-None-Match";
	HeaderName IfRange = "If-Range";
	HeaderName IfUnmodifiedSince = "If-Unmodified-Since";
	HeaderName MaxForwards = "Max-Forwards";
	HeaderName ProxyAuthorization = "Proxy-Authorization";
	HeaderName Range = "Range";
	HeaderName Referer = "Referer";
	HeaderName TE = "TE";
	HeaderName UserAgent = "User-Agent";
	
	// non-standard request headers
	HeaderName XRequestedWith = "X-Requested-With";
	HeaderName DNT = "DNT";
	HeaderName XForwardedFor = "X-Forwarded-For";
	
	// response headers
	HeaderName AcceptRanges = "Accept-Ranges";
	HeaderName Age = "Age";
	HeaderName ETag = "ETag";
	HeaderName Location = "Location";
	HeaderName ProxyAuthenticate = "Proxy-Authenticate";
	HeaderName RetryAfter = "Retry-After";
	HeaderName Server = "Server";
	HeaderName Vary = "Vary";
	HeaderName WWWAuthenticate = "WWW-Authenticate";
	
	// non-standard response headers
	HeaderName XFrameOptions = "X-Frame-Options";
	HeaderName XXSSProtection = "X-XSS-Protection";
	
	// cookies
	HeaderName Cookie = "Cookie";
	HeaderName SetCookie = "Set-Cookie";
}

class TASTEFULSERVER_API NameValuePair {
	public:
		NameValuePair();
		NameValuePair(const QString& name, const QString& value);
	
		QString getName() const;
		QString getValue() const;
	protected:
		QString name;
		QString value;
};

class TASTEFULSERVER_API HttpHeaderElementParameter : public NameValuePair {
	public:
		HttpHeaderElementParameter();
		HttpHeaderElementParameter(const QString& name, const QString& value);
	
		QString toString() const;
		
		static HttpHeaderElementParameter fromString(const QString& param);
	protected:
		static QString stripQuotes(const QString& string);
		void parse(const QString& param);
};

class TASTEFULSERVER_API HttpHeaderElement : public NameValuePair {
	public:
		HttpHeaderElement();
		HttpHeaderElement(const QString& name, const QString& value);
	
		bool isSet(const QString& paramName) const;
		QString getParameter(const QString& paramName) const;
	
		QList<HttpHeaderElementParameter> getParameters() const;
	
		static HttpHeaderElement fromString(const QString& elementString);
	
		QString toString() const;
	protected:
		QHash<QString, HttpHeaderElementParameter> parameters;
	
		void parse(const QString& elementString);
};

class TASTEFULSERVER_API HttpHeaderValue {
	public:
		HttpHeaderValue();
		HttpHeaderValue(const QString& value);
	
		bool isValid() const;
	
		HttpHeaderElement getElement() const;
		QList<HttpHeaderElement> getElements() const;
		void merge(const QList<HttpHeaderElement>& newElements);
	
		QString toString() const;
	protected:
		void parseElementsIfNecessary() const;
	
		QString value;
		bool modified;
		mutable QList<HttpHeaderElement> elements;
};

class TASTEFULSERVER_API HttpHeader {
	public:
		HttpHeader();
		HttpHeader(const QString& name, const QString& value);
	
		QString getName() const;
		QString getValue() const;
		void setValue(const QString& value);
	
		bool isValid() const;
	
		bool merge(const HttpHeader& header);
		QString toString() const;
		
		static HttpHeader forName(const QString& name);
		static HttpHeader fromString(const QString& header);
	
		HttpHeaderElement getElement() const;
		QList<HttpHeaderElement> getElements() const;
	protected:
		QString name;
		HttpHeaderValue value;
	
		void parse(const QString& headerString);
};
