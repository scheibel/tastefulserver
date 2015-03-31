#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpEntity.h>
#include <tastefulserver/HttpVersion.h>
#include <tastefulserver/Cookie.h>
#include <tastefulserver/ContentType.h>
#include <tastefulserver/MultiPart.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpMessage : public HttpEntity
{
public:
    HttpMessage();
    HttpMessage(const HttpVersion & httpVersion);

    const HttpVersion & getHttpVersion() const;
    HttpVersion & getHttpVersion();

    const Cookies & getCookies() const;
    Cookies & getCookies();
    bool hasCookies() const;

    const ContentType & getContentType() const;
    ContentType & getContentType();

    bool isMultiPart() const;
    const MultiPart & getMultiPart() const;
    MultiPart & getMultiPart();

    void keepAlive();
    bool isKeepAlive() const;

    int getContentLength() const;

protected:
    HttpVersion m_httpVersion;
    Cookies m_cookies;
    ContentType m_contentType;
    MultiPart m_multiPart;
};

} // namespace tastefulserver
