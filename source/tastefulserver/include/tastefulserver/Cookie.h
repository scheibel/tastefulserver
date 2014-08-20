#pragma once

#include <QString>
#include <QHash>

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/httpUtil.h>

namespace tastefulserver {

// TODO: Subclass NameValuePair?
class TASTEFULSERVER_API Cookie
{
public:
    static const QString MaxAge;
    static const QString Comment;
    static const QString Domain;
    static const QString Path;
    static const QString Version;
    static const QString Expires;
    static const QString Secure;
    static const QString HttpOnly;

public:
    Cookie();
    Cookie(const QString & name, const QString & value);

    const QString & getName() const;
    const QString & getValue() const;

    Cookie & setValue(const QString & value);
    Cookie & setMaxAge(unsigned seconds);
    Cookie & setComment(const QString & comment);
    Cookie & setDomain(const QString & domain);
    Cookie & setPath(const QString & path);
    Cookie & setVersion(const QString & version);
    Cookie & setExpires(const QDateTime & dateTime);
    Cookie & setExpires(const QString & expires);
    Cookie & setSecure(bool secure);
    Cookie & setHttpOnly(bool httpOnly);

    QString toString() const;

private:
    QString m_name;
    QString m_value;
    bool m_secure;
    bool m_httpOnly;
    QHash<QString, QString> m_attributes;
};

class TASTEFULSERVER_API Cookies
{
public:
    bool isEmpty() const;

    void set(const Cookie & cookie);
    bool has(const QString & name) const;
    Cookie get(const QString & name) const;
    Cookie & operator[](const QString & name);
    const Cookie operator[](const QString & name) const;
    void clear(Cookie & cookie);

    void clear();

    QHash<QString, Cookie>::iterator begin();
    QHash<QString, Cookie>::const_iterator begin() const;
    QHash<QString, Cookie>::iterator end();
    QHash<QString, Cookie>::const_iterator end() const;

    void parse(const QString & cookiesString);

    static Cookies fromString(const QString & cookiesString);

private:
    QHash<QString, Cookie> m_cookies;
};

} // namespace tastefulserver
