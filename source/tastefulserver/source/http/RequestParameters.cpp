#include <tastefulserver/RequestParameters.h>

#include <QUrl>
#include <QByteArray>
#include <QStringList>

#include <tastefulserver/MultiPart.h>
#include <tastefulserver/UploadedFile.h>
#include <tastefulserver/ByteStream.h>

namespace tastefulserver {

RequestParameters::RequestParameters()
{
}

void RequestParameters::clear()
{
    m_params.clear();
    m_files.clear();
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

void RequestParameters::parseUrl(const QUrl & url)
{
    if (!url.hasQuery())
    {
        return;
    }

    for (const QString & parameter : url.query(QUrl::FullyDecoded).split('&'))
    {
        int splitIndex = parameter.indexOf('=');
        if (splitIndex<=0)
        {
            continue;
        }

        QString key = parameter.left(splitIndex);
        QString value = parameter.mid(splitIndex + 1);

        m_params[key] = value;
    }
}

void RequestParameters::parseUrlEncoded(const QByteArray & urlEncoded)
{
    QByteArray copy(urlEncoded);

    parseUrl(QUrl::fromPercentEncoding("/?" + copy.replace('+', ' ')));
}

void RequestParameters::parseMultiPart(const MultiPart & multiPart)
{
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

                m_files[name] = uploadedFile;
            }
            else
            {
                m_params[name] = QString(part.getContent());
            }
        }
    }
}

QString RequestParameters::get(const QString & key) const
{
    return m_params.value(key, "");
}

bool RequestParameters::contains(const QString & key) const
{
    return m_params.contains(key);
}

UploadedFile RequestParameters::getFile(const QString & key) const
{
    return m_files.value(key);
}

bool RequestParameters::containsFile(const QString & key) const
{
    return m_files.contains(key);
}

const QHash<QString, QString> & RequestParameters::parameters() const
{
    return m_params;
}

const QHash<QString, UploadedFile> & RequestParameters::files() const
{
    return m_files;
}

} // namespace tastefulserver
