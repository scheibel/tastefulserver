#include <tastefulserver/ContentType.h>

namespace tastefulserver {

const QString ContentType::Charset = "charset";
const QString ContentType::Boundary = "boundary";
const QString ContentType::Multipart = "multipart";
const QString ContentType::Application = "application";
const QString ContentType::FormData = "form-data";
const QString ContentType::XWWWFormUrlEncoded = "x-www-form-urlencoded";
ContentType::ContentType()
{
}

ContentType::ContentType(const QString & type, const QString & subtype)
    : m_type(type)
    , m_subtype(subtype)
{
    updateName();
}

void ContentType::updateName()
{
    m_name = m_type + '/' + m_subtype;
}

bool ContentType::is(const QString & type, const QString & subtype) const
{
    return m_type==type && m_subtype==subtype;
}

bool ContentType::operator==(const ContentType & contentType) const
{
    return m_type == contentType.m_type && m_subtype == contentType.m_subtype;
}

void ContentType::setType(const QString & type)
{
    m_type = type;

    updateName();
}

void ContentType::setSubtype(const QString & subtype)
{
    m_subtype = subtype;

    updateName();
}

void ContentType::setTypeAndSubtype(const QString & typeAndSubtype)
{
    int pos = typeAndSubtype.indexOf('/');

    if (pos<0)
    {
        m_type = typeAndSubtype;
        m_subtype = QString();
    }
    else
    {
        m_type = typeAndSubtype.left(pos);
        m_subtype = typeAndSubtype.mid(pos + 1);
    }

    updateName();
}

ContentType ContentType::fromString(const QString & value)
{
    ContentType contentType;

    contentType.parse(value);

    return contentType;
}

void ContentType::parse(const QString & value)
{
    HttpHeaderElement::parse(value);

    setTypeAndSubtype(m_name);
}

const QString &ContentType::getType() const
{
    return m_type;
}

const QString &ContentType::getSubtype() const
{
    return m_subtype;
}

QString ContentType::getCharset() const
{
    return getParameter(Charset);
}

QString ContentType::getBoundary() const
{
    return getParameter(Boundary);
}

bool ContentType::isMultiPart() const
{
    return m_type==Multipart;
}

} // namespace tastefulserver
