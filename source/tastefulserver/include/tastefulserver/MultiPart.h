#pragma once

#include <QString>
#include <QList>
#include <QHash>

#include <tastefulserver/tasteful-server_api.h>

#include <tastefulserver/HttpHeader.h>
#include <tastefulserver/ContentType.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API Part
{
public:
    Part();
    Part(const QList<HttpHeader> & headers, const QByteArray & content);

    bool hasHeader(const http::HeaderName & headerName) const;
    HttpHeader getHeader(const http::HeaderName & headerName) const;

    QList<HttpHeader> getHeaders() const;
    QByteArray getContent() const;

protected:
    QHash<QString, HttpHeader> m_headers;
    QByteArray m_content;
};

class TASTEFUL_SERVER_API MultiPart
{
public:
    MultiPart();
    MultiPart(const ContentType & type);

    void parse(const QByteArray & content);

    bool isFormData() const;

    QList<Part> getParts() const;

protected:
    ContentType m_type;
    QList<Part> m_parts;
};

} // namespace tastefulserver
