#include <tastefulserver/UploadedFile.h>

#include <QVariant>

namespace tastefulserver {

UploadedFile::UploadedFile()
{
}

UploadedFile::UploadedFile(const ContentType & contentType)
    : m_contentType(contentType)
{
}

QVariant UploadedFile::toQVariant() const
{
    return QVariant::fromValue(*this);
}

void UploadedFile::setFilename(const QString & filename)
{
    m_filename = filename;
}

void UploadedFile::setContent(const QByteArray & content)
{
    m_content = content;
}

const QString &UploadedFile::getFilename() const
{
    return m_filename;
}

const QByteArray &UploadedFile::getContent() const
{
    return m_content;
}

const ContentType &UploadedFile::getContentType() const
{
    return m_contentType;
}

} // namespace tastefulserver
