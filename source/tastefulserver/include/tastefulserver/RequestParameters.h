#pragma once

#include <QSharedPointer>

#include <tastefulserver/tastefulserver_api.h>

class QString;
class QByteArray;

namespace tastefulserver {

class QVariantAbstractTree;
class QVariantTree;
class MultiPart;

class TASTEFULSERVER_API RequestParameters
{
public:
    RequestParameters();

    bool contains(const QString & key) const;
    bool containsPath(const QString & path) const;

    QVariantAbstractTree & operator[](const QString & key) const;
    QVariantAbstractTree & get(const QString & key) const;
    QVariantAbstractTree & getByPath(const QString & path) const;

    void insert(const QString & key, const QVariant & value);

    void parseUrl(const QUrl & url);
    void parseUrlEncoded(const QByteArray & urlEncodedPost);
    void parseMultiPart(const MultiPart & multiPart);

    QString toString() const;

    static RequestParameters fromUrl(const QUrl & url);
    static RequestParameters fromUrlEncoded(const QByteArray & urlEncoded);
    static RequestParameters fromMultiPart(const MultiPart & multiPart);

protected:
    void parseList(const QList<QPair<QString, QVariant >> &parameters);
    QList<QString> extractIndices(const QString & key) const;

    QSharedPointer<QVariantTree> m_params;
};

} // namespace tastefulserver
