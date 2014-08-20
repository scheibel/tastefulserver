#include <tasteful-server/HttpMethod.h>

#include <QString>
#include <QHash>

namespace tastefulserver {

HttpMethod::HttpMethod()
    : m_method(INVALID)
{
}

HttpMethod::HttpMethod(const Method & method)
    : m_method(method)
{
}

HttpMethod::HttpMethod(const HttpMethod & method)
    : m_method(method.m_method)
{
}

HttpMethod HttpMethod::fromString(const QString & methodString)
{
    static QHash<QString, Method> hash = makeHash();

    return HttpMethod(hash.value(methodString.toUpper(), INVALID));
}

QHash<QString, HttpMethod::Method> HttpMethod::makeHash()
{
    QHash<QString, Method> hash;

    hash.insert("GET", GET);
    hash.insert("POST", POST);
    hash.insert("PUT", PUT);
    hash.insert("DELETE", DELETE);
    hash.insert("UPDATE", UPDATE);
    hash.insert("TRACE", TRACE);
    hash.insert("HEAD", HEAD);
    hash.insert("OPTIONS", OPTIONS);
    hash.insert("CONNECT", CONNECT);

    return hash;
}

HttpMethod &HttpMethod::operator=(const Method & method)
{
    m_method = method;

    return *this;
}

HttpMethod &HttpMethod::operator=(const HttpMethod & method)
{
    m_method = method.m_method;

    return *this;
}

bool HttpMethod::operator==(const Method & method) const
{
    return m_method == method;
}

bool HttpMethod::operator==(const HttpMethod & method) const
{
    return m_method == method.m_method;
}

bool HttpMethod::operator!=(const Method & method) const
{
    return m_method != method;
}

bool HttpMethod::operator!=(const HttpMethod & method) const
{
    return m_method != method.m_method;
}

HttpMethod::operator unsigned() const
{
    return m_method;
}

HttpMethod::operator Method() const
{
    return m_method;
}

bool HttpMethod::isInvalid() const
{
    return m_method == INVALID;
}

HttpMethod::Method HttpMethod::method() const
{
    return m_method;
}

QString HttpMethod::toString() const
{
    switch (m_method)
    {
        case GET:

            return "GET";
        case POST:

            return "POST";
        case PUT:

            return "PUT";
        case DELETE:

            return "DELETE";
        case UPDATE:

            return "UPDATE";
        case TRACE:

            return "TRACE";
        case HEAD:

            return "HEAD";
        case OPTIONS:

            return "OPTIONS";
        case CONNECT:

            return "CONNECT";
        default:

            return "INVALID";
    }
}

} // namespace tastefulserver
