#pragma once

#include <QHash>

#include <tasteful-server/tasteful-server_api.h>
#include <tasteful-server/UploadedFile.h>

class QString;
class QByteArray;
class QUrl;

namespace tastefulserver {

class MultiPart;

class TASTEFUL_SERVER_API RequestParameters
{
public:
    RequestParameters();

    void clear();

    bool contains(const QString & key) const;
    QString get(const QString & key) const;

    bool containsFile(const QString & key) const;
    UploadedFile getFile(const QString & key) const;

    const QHash<QString, QString> & parameters() const;
    const QHash<QString, UploadedFile> & files() const;

    void parseUrl(const QUrl & url);
    void parseUrlEncoded(const QByteArray & urlEncodedPost);
    void parseMultiPart(const MultiPart & multiPart);

    static RequestParameters fromUrl(const QUrl & url);
    static RequestParameters fromUrlEncoded(const QByteArray & urlEncoded);
    static RequestParameters fromMultiPart(const MultiPart & multiPart);

protected:
    QHash<QString, QString> m_params;
    QHash<QString, UploadedFile> m_files;
};

} // namespace tastefulserver
