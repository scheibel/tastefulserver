#pragma once

#include <QString>

#include <tastefulserver/tasteful-server_api.h>

#include <tastefulserver/HttpHeader.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API ContentType : public HttpHeaderElement
{
public:
    static const QString Charset;
    static const QString Boundary;
    static const QString Multipart;
    static const QString Application;
    static const QString FormData;
    static const QString XWWWFormUrlEncoded;

    ContentType();
    ContentType(const QString & type, const QString & subtype);
    static ContentType fromString(const QString & value);

    bool is(const QString & type, const QString & subtype) const;
    bool operator==(const ContentType & contentType) const;

    void setType(const QString & type);
    void setSubtype(const QString & subtype);
    void setTypeAndSubtype(const QString & typeAndSubype);

    const QString & getType() const;
    const QString & getSubtype() const;

    QString getCharset() const;
    QString getBoundary() const;

    bool isMultiPart() const;

    void parse(const QString & value);

protected:
    void updateName();

    QString m_type;
    QString m_subtype;
};

} // namespace tastefulserver
