#include <tastefulserver/HttpMessage.h>

#include <tastefulserver/HttpHeader.h>

namespace tastefulserver {

HttpVersion::HttpVersion()
    : m_major(0)
    , m_minor(0)
{
}

HttpVersion::HttpVersion(unsigned majorVersion, unsigned minorVersion)
    : m_major(majorVersion)
    , m_minor(minorVersion)
{
}

bool HttpVersion::isInvalid() const
{
    return m_major==0 && m_minor==0;
}

unsigned HttpVersion::getMajor() const
{
    return m_major;
}

unsigned HttpVersion::getMinor() const
{
    return m_minor;
}

bool HttpVersion::operator<(const HttpVersion & httpVersion) const
{
    return m_major<httpVersion.m_major && m_minor<httpVersion.m_minor;
}

bool HttpVersion::operator<=(const HttpVersion & httpVersion) const
{
    return m_major<=httpVersion.m_major && m_minor<=httpVersion.m_minor;
}

bool HttpVersion::operator>(const HttpVersion & httpVersion) const
{
    return m_major>httpVersion.m_major && m_minor>httpVersion.m_minor;
}

bool HttpVersion::operator>=(const HttpVersion & httpVersion) const
{
    return m_major>=httpVersion.m_major && m_minor>=httpVersion.m_minor;
}

bool HttpVersion::operator==(const HttpVersion & httpVersion) const
{
    return m_major==httpVersion.m_major && m_minor==httpVersion.m_minor;
}

bool HttpVersion::operator!=(const HttpVersion & httpVersion) const
{
    return m_major!=httpVersion.m_major || m_minor!=httpVersion.m_minor;
}

QString HttpVersion::toString() const
{
    return QString("HTTP/%1.%2").arg(m_major).arg(m_minor);
}

HttpVersion HttpVersion::fromString(const QString & string)
{
    if (!string.startsWith("HTTP/"))
    {
        return HttpVersion();
    }
    QString version = string.mid(5);
    int pos = version.indexOf('.');
    if (pos<0)
    {
        return HttpVersion();
    }
    unsigned _major = version.left(pos).toUInt();
    unsigned _minor = version.mid(pos + 1).toUInt();

    return HttpVersion(_major, _minor);
}

} // namespace tastefulserver
