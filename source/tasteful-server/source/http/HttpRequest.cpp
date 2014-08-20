#include <tasteful-server/HttpRequest.h>

#include <QIODevice>

namespace tastefulserver {

HttpRequest::HttpRequest()
: m_port(0)
{
}

HttpRequest::HttpRequest(const HttpMethod & method, const QString & requestUri, const HttpVersion & httpVersion)
: HttpMessage(httpVersion)
, m_port(0)
, m_method(method)
, m_requestUri(requestUri)
{
    m_url = QUrl::fromEncoded(requestUri.toLatin1());
    if (m_url.scheme().isEmpty())
    {
        m_url.setScheme("http");
    }
    m_requestParams.parseUrl(m_url);
}

void HttpRequest::setHttps(bool isHttps)
{
    m_url.setScheme(isHttps ? "https" : "http");
}

const QHostAddress &HttpRequest::address() const
{
    return m_address;
}

void HttpRequest::setAddress(const QHostAddress & address)
{
    m_address = address;
}

unsigned HttpRequest::port() const
{
    return m_port;
}

void HttpRequest::setPort(unsigned port)
{
    m_port = port;
}

bool HttpRequest::isXMLHttpRequest() const
{
    return getHeader(http::XRequestedWith).getValue()=="XMLHttpRequest";
}

HttpMethod HttpRequest::getMethod() const
{
    return m_method;
}

QString HttpRequest::getRequestUri() const
{
    return m_requestUri;
}

QUrl HttpRequest::getUrl() const
{
    return m_url;
}

QString HttpRequest::getPath() const
{
    return m_url.path();
}

RequestParameters &HttpRequest::getParameters()
{
    return m_requestParams;
}

const RequestParameters &HttpRequest::getParameters() const
{
    return m_requestParams;
}

void HttpRequest::writeTo(QIODevice & device) const
{
    device.write((m_method.toString() + " " + m_requestUri + " " + m_httpVersion.toString() + http::Linebreak).toLocal8Bit());

    HttpMessage::writeTo(device);
}

void HttpRequest::finalize()
{
    interpretHeaders();
    interpretContent();
}

void HttpRequest::interpretHeaders()
{
    if (hasHeader(http::Host))
    {
        m_url.setAuthority(getHeader(http::Host).getValue());
    }

    if (hasHeader(http::Cookie))
    {
        m_cookies.clear();

        for (const HttpHeader & header : getHeaders(http::Cookie))
        {
            m_cookies.parse(header.getValue());
        }
    }

    if (hasHeader(http::ContentType))
    {
        m_contentType.parse(getHeader(http::ContentType).getValue());
    }
}

void HttpRequest::interpretContent()
{
    if (isMultiPart())
    {
        m_multiPart = MultiPart(m_contentType);
        m_multiPart.parse(getContent());

        if (m_multiPart.isFormData())
        {
            m_requestParams.clear();
            m_requestParams.parseMultiPart(m_multiPart);
        }
    }
    else if (m_contentType.is(ContentType::Application, ContentType::XWWWFormUrlEncoded))
    {
        m_requestParams.clear();
        m_requestParams.parseUrlEncoded(getContent());
    }
}

} // namespace tastefulserver
