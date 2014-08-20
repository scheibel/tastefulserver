#pragma once

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/HttpMessage.h>
#include <tasteful-server/httpStatusCodes.h>
#include <tasteful-server/HttpRequest.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API HttpResponse : public HttpMessage
{
public:
    HttpResponse(unsigned statusCode);
    HttpResponse(unsigned statusCode, const HttpVersion & httpVersion);
    HttpResponse(unsigned statusCode, const HttpRequest & httpRequest);

    void setStatusCode(unsigned statusCode);
    unsigned getStatusCode() const;

    void setContentType(const QString & contentType);
    void setDate();
    void setDate(const QDateTime & date);

    void setCookie(const Cookie & cookie);
    Cookie & setCookie(const QString & key, const QString & value);
   
    virtual void writeTo(QIODevice & device) const;
protected:
    unsigned m_statusCode;

    virtual void writeHeadersOn(QIODevice & device) const;
};

} // namespace tastefulserver
