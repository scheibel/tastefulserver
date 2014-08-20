#pragma once

#include <tastefulserver/tastefulserver_api.h>

class QString;

namespace tastefulserver {

class TASTEFULSERVER_API HttpVersion
{
public:
    HttpVersion();
    HttpVersion(unsigned majorVersion, unsigned minorVersion);

    bool isInvalid() const;
    unsigned getMajor() const;
    unsigned getMinor() const;

    bool operator<(const HttpVersion & httpVersion) const;
    bool operator<=(const HttpVersion & httpVersion) const;
    bool operator>(const HttpVersion & httpVersion) const;
    bool operator>=(const HttpVersion & httpVersion) const;
    bool operator==(const HttpVersion & httpVersion) const;
    bool operator!=(const HttpVersion & httpVersion) const;

    QString toString() const;

    static HttpVersion fromString(const QString & string);

private:
    unsigned m_major;
    unsigned m_minor;
};

} // namespace tastefulserver
