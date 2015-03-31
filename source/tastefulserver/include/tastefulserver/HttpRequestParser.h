#pragma once

#include <tastefulserver/tasteful-server_api.h>

#include <tastefulserver/ByteStream.h>
#include <tastefulserver/HttpRequest.h>

#include <QObject>
#include <QQueue>

namespace tastefulserver {

class TASTEFUL_SERVER_API HttpRequestParser : public QObject
{
    Q_OBJECT
public:
    HttpRequestParser();
    virtual ~HttpRequestParser();

    void addData(const QByteArray & data);

    bool hasReadyRequests() const;
    HttpRequest popReadyRequest();

signals:
    void badRequest();

protected:
    enum class ParseState
    {
        RequestLine,
        HeaderLine,
        Content,
        Finish,
        Error,
        Interrupted
    };

    void parse();

    ParseState dispatch(ParseState state);

    ParseState parseRequestLine();
    ParseState parseHeaderLine();
    ParseState parseContent();
    ParseState finishRequest();
    ParseState handleError();

    void pushRequest();

    ByteStream m_byteStream;
    HttpRequest m_currentRequest;
    ParseState m_state;
    ParseState m_interruptedState;
    QQueue<HttpRequest> m_readyRequests;
};

} // namespace tastefulserver
