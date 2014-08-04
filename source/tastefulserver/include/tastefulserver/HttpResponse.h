#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpMessage.h>
#include <tastefulserver/httpStatusCodes.h>
#include <tastefulserver/HttpRequest.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpResponse : public HttpMessage
{
public:
    HttpResponse();
    HttpResponse(const HttpVersion & httpVersion);
    HttpResponse(const HttpRequest & httpRequest);

    void setStatusCode(unsigned statusCode);
    unsigned getStatusCode() const;

    void setDate(const QDateTime & date);

    void setCookie(const Cookie & cookie);
    Cookie & setCookie(const QString & key, const QString & value);
    virtual QByteArray toByteArray() const;

protected:
    unsigned m_statusCode;

    void initialize();

    virtual void writeHeadersOn(QTextStream & stream) const;
};

} // namespace tastefulserver
