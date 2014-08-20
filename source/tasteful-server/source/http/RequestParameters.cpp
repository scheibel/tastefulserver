#include <tasteful-server/RequestParameters.h>

#include <QUrl>
#include <QRegExp>
#include <QByteArray>

#include <tasteful-server/MultiPart.h>
#include <tasteful-server/UploadedFile.h>
#include <tasteful-server/ByteStream.h>
#include <tasteful-server/QVariantTree.h>

namespace tastefulserver {

RequestParameters::RequestParameters()
: m_params(new QVariantTree())
{
}

void RequestParameters::clear()
{
    m_params.clear();
}

RequestParameters RequestParameters::fromUrl(const QUrl & url)
{
    RequestParameters params;

    params.parseUrl(url);

    return params;
}

RequestParameters RequestParameters::fromUrlEncoded(const QByteArray & urlEncodedPost)
{
    RequestParameters params;

    params.parseUrlEncoded(urlEncodedPost);

    return params;
}

RequestParameters RequestParameters::fromMultiPart(const MultiPart & multiPart)
{
    RequestParameters params;

    params.parseMultiPart(multiPart);

    return params;
}

QString RequestParameters::toString() const
{
    return m_params->printString();
}

void RequestParameters::parseUrl(const QUrl & url)
{
    if (!url.hasQuery())
    {
        return;
    }

    QList<QPair<QString, QVariant >> parameters;

    for (const QString & parameter : url.query().split('&'))
    {
        int splitIndex = parameter.indexOf('=');
        if (splitIndex<=0)
        {
            continue;
        }

        QString key = parameter.left(splitIndex);
        QString value = parameter.mid(splitIndex + 1);

        parameters << QPair<QString, QVariant>(key, value);
    }

    parseList(parameters);
}

void RequestParameters::parseUrlEncoded(const QByteArray & urlEncoded)
{
    QByteArray copy(urlEncoded);

    parseUrl(QUrl::fromPercentEncoding("/?" + copy.replace('+', ' ')));
}

void RequestParameters::parseMultiPart(const MultiPart & multiPart)
{
    QList<QPair<QString, QVariant >> parameters;

    for (const Part & part : multiPart.getParts())
    {
        HttpHeader contentDisposition = part.getHeader(http::ContentDisposition);
        HttpHeaderElement element = contentDisposition.getElement();
        if (element.getName()=="form-data")
        {
            QString name = element.getParameter("name");
            if (part.hasHeader(http::ContentType))
            {
                UploadedFile uploadedFile;
                uploadedFile.setContent(part.getContent());
                if (element.isSet("filename"))
                {
                    uploadedFile.setFilename(element.getParameter("filename"));
                }
                parameters << QPair<QString, QVariant>(name, uploadedFile.toQVariant());
            }
            else
            {
                parameters << QPair<QString, QVariant>(name, QString(part.getContent()));
            }
        }
    }

    parseList(parameters);
}

void RequestParameters::parseList(const QList<QPair<QString, QVariant >> & parameters)
{
    for (const QPair<QString, QVariant> & pair : parameters)
    {
        if (pair.first.isEmpty())
        {
            continue;
        }

        QList<QString> indices = extractIndices(pair.first);

        QVariantTree * currentParams = m_params.data();
        QString index;
        for (int i = 0;i < indices.size() - 1;++i)
        {
            index = indices[i].isEmpty() ? QString::number(currentParams->size()) : indices[i];
            currentParams = &currentParams->obtainSubtree(index);
        }

        index = indices.last().isEmpty() ? QString::number(currentParams->size()) : indices.last();
        currentParams->insert(index, pair.second);
    }
}

QVariantTree & RequestParameters::tree()
{
    return *m_params.data();
}

const QVariantTree & RequestParameters::tree() const
{
    return *m_params.data();
}

QVariantAbstractTree &RequestParameters::operator[](const QString & key) const
{
    return get(key);
}

QVariantAbstractTree &RequestParameters::getByPath(const QString & path) const
{
    return m_params->getByPath(path);
}

QVariantAbstractTree &RequestParameters::get(const QString & key) const
{
    return m_params->get(key);
}

void RequestParameters::insert(const QString & key, const QVariant & value)
{
    m_params->insert(key, value);
}

bool RequestParameters::contains(const QString & key) const
{
    return m_params->contains(key);
}

bool RequestParameters::containsPath(const QString & path) const
{
    return m_params->containsPath(path);
}

QList<QString> RequestParameters::extractIndices(const QString & key) const
{
    ByteStream stream(key.toUtf8());
    QString name = stream.readUpTo('[');

    if (name.isEmpty())
    {
        return QList<QString>() << key;
    }

    QList<QString> indices = QList<QString>() << name;

    while (stream.canReadUpTo('['))
    {
        stream.skipBehind('[');

        if (stream.canReadUpTo(']'))
        {
            indices << stream.readUpTo(']', true);
        }
        else
        {
            return QList<QString>() << key;
        }
    }

    if (!stream.atEnd())
    {
        return QList<QString>() << key;
    }

    return indices;
}

} // namespace tastefulserver
