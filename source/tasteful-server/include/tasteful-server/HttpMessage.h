#pragma once

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/HttpEntity.h>
#include <tasteful-server/HttpVersion.h>
#include <tasteful-server/Cookie.h>
#include <tasteful-server/ContentType.h>
#include <tasteful-server/MultiPart.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API HttpMessage : public HttpEntity
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
