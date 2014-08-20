#include <tasteful-server/Cookie.h>

#include <QTextStream>
#include <QStringList>
#include <QDateTime>

#include <tasteful-server/HttpHeader.h>

namespace tastefulserver {

const QString Cookie::MaxAge = "Max-Age";
const QString Cookie::Comment = "Comment";
const QString Cookie::Domain = "Domain";
const QString Cookie::Path = "Path";
const QString Cookie::Version = "Version";
const QString Cookie::Expires = "Expires";
const QString Cookie::Secure = "Secure";
const QString Cookie::HttpOnly = "HttpOnly";

Cookie::Cookie()
: m_secure(false)
, m_httpOnly(false)
{
}

Cookie::Cookie(const QString & name, const QString & value)
: m_name(name)
, m_value(value)
, m_secure(false)
, m_httpOnly(false)
{
}

const QString & Cookie::getName() const
{
    return m_name;
}

const QString & Cookie::getValue() const
{
    return m_value;
}

Cookie & Cookie::setValue(const QString & value)
{
    m_value = value;

    return *this;
}

Cookie & Cookie::setMaxAge(unsigned seconds)
{
    m_attributes[MaxAge] = QString::number(seconds);

    return *this;
}

Cookie & Cookie::setComment(const QString & comment)
{
    m_attributes[Comment] = comment;

    return *this;
}

Cookie & Cookie::setDomain(const QString & domain)
{
    m_attributes[Domain] = domain;

    return *this;
}

Cookie & Cookie::setPath(const QString & path)
{
    m_attributes[Path] = path;

    return *this;
}

Cookie & Cookie::setVersion(const QString & version)
{
    m_attributes[Version] = version;

    return *this;
}

Cookie & Cookie::setExpires(const QDateTime & dateTime)
{
    return setExpires(http::dateString(dateTime));
}

Cookie & Cookie::setExpires(const QString & expires)
{
    m_attributes[Expires] = expires;

    return *this;
}

Cookie & Cookie::setSecure(bool secure)
{
    m_secure = secure;

    return *this;
}

Cookie & Cookie::setHttpOnly(bool httpOnly)
{
    m_httpOnly = httpOnly;

    return *this;
}

QString Cookie::toString() const
{
    if (m_name.isNull())
    {
        return QString();
    }

    QString string;
    QTextStream stream(&string);

    stream << m_name << "=" << m_value;

    for (QString & attribute : m_attributes.keys())
    {
        stream << "; " << attribute << "=" << m_attributes[attribute];
    }
    if (m_secure)
    {
        stream << "; " << Secure;
    }
    if (m_httpOnly)
    {
        stream << "; " << HttpOnly;
    }

    return string;
}

bool Cookies::isEmpty() const
{
    return m_cookies.isEmpty();
}

void Cookies::set(const Cookie & cookie)
{
    m_cookies[cookie.getName()] = cookie;
}

bool Cookies::has(const QString & name) const
{
    return m_cookies.contains(name);
}

Cookie Cookies::get(const QString & name) const
{
    return has(name) ? m_cookies[name] : Cookie();
}

Cookie &Cookies::operator[](const QString & name)
{
    return m_cookies[name];
}

const Cookie Cookies::operator[](const QString & name) const
{
    return m_cookies[name];
}

void Cookies::clear(Cookie & cookie)
{
    cookie.setValue("deleted");
    cookie.setExpires(QDateTime::fromTime_t(0));
    m_cookies[cookie.getName()] = cookie;
}

void Cookies::clear()
{
    m_cookies.clear();
}

Cookies Cookies::fromString(const QString & cookiesString)
{
    Cookies cookies;

    cookies.parse(cookiesString);

    return cookies;
}

void Cookies::parse(const QString & cookiesString)
{
    QStringList parts = cookiesString.split(";");

    for (QString & part : parts)
    {
        if (!part.contains("="))
        {
            continue;
        }
        QStringList nameValue = part.trimmed().split("=");
        set(Cookie(nameValue[0], nameValue[1]));
    }
}

QHash<QString, Cookie>::iterator Cookies::begin()
{
    return m_cookies.begin();
}

QHash<QString, Cookie>::const_iterator Cookies::begin() const
{
    return m_cookies.begin();
}

QHash<QString, Cookie>::iterator Cookies::end()
{
    return m_cookies.end();
}

QHash<QString, Cookie>::const_iterator Cookies::end() const
{
    return m_cookies.end();
}

} // namespace tastefulserver
