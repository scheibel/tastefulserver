#pragma once

#include <QString>
#include <QByteArray>
#include <QMetaType>

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/ContentType.h>

class QVariant;

namespace tastefulserver {

class TASTEFUL_SERVER_API UploadedFile
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
