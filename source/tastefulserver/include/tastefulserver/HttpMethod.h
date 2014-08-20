#pragma once

#include <tastefulserver/tastefulserver_api.h>

class QString;
template <typename Key, typename Value>
class QHash;

namespace tastefulserver {

class TASTEFULSERVER_API HttpMethod
{
public:
    enum Method
    {
        INVALID, GET, POST, PUT, DELETE, UPDATE, TRACE, HEAD, OPTIONS, CONNECT
    };

    HttpMethod();
    HttpMethod(const Method & method);
    HttpMethod(const HttpMethod & method);

    static HttpMethod fromString(const QString & methodString);
    HttpMethod &operator=(const Method & method);
    HttpMethod &operator=(const HttpMethod & method);

    bool operator==(const Method & method) const;
    bool operator==(const HttpMethod & method) const;

    bool operator!=(const Method & method) const;
    bool operator!=(const HttpMethod & method) const;

    operator unsigned() const;
    operator Method() const;

    bool isInvalid() const;

    Method method() const;
    QString toString() const;

private:
    Method m_method;
    static QHash<QString, Method> makeHash();
};

} // namespace tastefulserver
