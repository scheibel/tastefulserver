#pragma once

#include <QString>
#include <QList>
#include <QHash>

#include <tasteful-server/tasteful-server_api.h>

namespace tastefulserver {

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

// websockets
HeaderName SecWebSocketKey = "Sec-WebSocket-Key";
HeaderName SecWebSocketAccept = "Sec-WebSocket-Accept";
HeaderName SecWebSocketVersion = "Sec-WebSocket-Version";
HeaderName SecWebSocketProtocol = "Sec-WebSocket-Protocol";
HeaderName SecWebSocketExtensions = "Sec-WebSocket-Extensions";
HeaderName Origin = "Origin";
}

class TASTEFUL_SERVER_API NameValuePair
{
public:
    NameValuePair();
    NameValuePair(const QString & name, const QString & value);

    const QString & getName() const;
    const QString & getValue() const;

protected:
    QString m_name;
    QString m_value;
};

class TASTEFUL_SERVER_API HttpHeaderElementParameter : public NameValuePair
{
public:
    HttpHeaderElementParameter();
    HttpHeaderElementParameter(const QString & name, const QString & value);

    QString toString() const;

    static HttpHeaderElementParameter fromString(const QString & param);

protected:
    static QString stripQuotes(const QString & string);
    void parse(const QString & param);
};

class TASTEFUL_SERVER_API HttpHeaderElement : public NameValuePair
{
public:
    HttpHeaderElement();
    HttpHeaderElement(const QString & name, const QString & value);

    bool isSet(const QString & paramName) const;
    QString getParameter(const QString & paramName) const;

    QList<HttpHeaderElementParameter> getParameters() const;

    static HttpHeaderElement fromString(const QString & elementString);

    QString toString() const;

protected:
    QHash<QString, HttpHeaderElementParameter> m_parameters;

    void parse(const QString & elementString);
};

class TASTEFUL_SERVER_API HttpHeaderValue
{
public:
    HttpHeaderValue();
    HttpHeaderValue(const QString & value);

    bool isValid() const;

    HttpHeaderElement getElement() const;
    QList<HttpHeaderElement> getElements() const;
    void merge(const QList<HttpHeaderElement> & newElements);

    QString toString() const;

protected:
    void parseElementsIfNecessary() const;

    QString m_value;
    bool m_modified;
    mutable QList<HttpHeaderElement> m_elements;
};

class TASTEFUL_SERVER_API HttpHeader
{
public:
    HttpHeader();
    HttpHeader(const QString & name, const QString & value);

    QString getName() const;
    QString getValue() const;
    void setValue(const QString & value);

    bool isValid() const;

    bool merge(const HttpHeader & header);
    QString toString() const;

    static HttpHeader forName(const QString & name);
    static HttpHeader fromString(const QString & header);

    HttpHeaderElement getElement() const;
    QList<HttpHeaderElement> getElements() const;

protected:
    QString m_name;
    HttpHeaderValue m_value;

    void parse(const QString & headerString);
};

} // namespace tastefulserver
