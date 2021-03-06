#pragma once

#include <QString>
#include <QByteArray>
#include <QMetaType>

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/ContentType.h>

class QVariant;

namespace tastefulserver {

class TASTEFULSERVER_API UploadedFile
{
public:
    UploadedFile();
    UploadedFile(const ContentType & contentType);

    void setFilename(const QString & filename);
    void setContent(const QByteArray & content);

    const QString & getFilename() const;
    const QByteArray & getContent() const;
    const ContentType & getContentType() const;

    QVariant toQVariant() const;

private:
    QString m_filename;
    QByteArray m_content;
    ContentType m_contentType;
};

} // namespace tastefulserver

Q_DECLARE_METATYPE(tastefulserver::UploadedFile)
