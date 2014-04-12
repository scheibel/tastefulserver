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

#include <tastefulserver/HttpEntity.h>
#include <tastefulserver/HttpVersion.h>
#include <tastefulserver/HttpHeader.h>
#include <tastefulserver/Cookie.h>
#include <tastefulserver/ContentType.h>
#include <tastefulserver/MultiPart.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpMessage : public HttpEntity {
	public:
		HttpMessage();
		HttpMessage(const HttpVersion& httpVersion);
	
		const HttpVersion& getHttpVersion() const;
		HttpVersion& getHttpVersion();
	
		const Cookies& getCookies() const;
		Cookies& getCookies();
		bool hasCookies() const;
		
		const ContentType& getContentType() const;
		ContentType& getContentType();
		bool isMultiPart() const;
		const MultiPart& getMultiPart() const;
		MultiPart& getMultiPart();
		void keepAlive();
		bool isKeepAlive() const;
		int getContentLength() const;
	protected:
		HttpVersion httpVersion;
		Cookies cookies;
		ContentType contentType;
		MultiPart multiPart;
};

} // namespace tastefulserver
