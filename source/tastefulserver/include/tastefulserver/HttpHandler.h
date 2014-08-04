#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/ConnectionHandler.h>
#include <tastefulserver/ByteArrayStream.h>
#include <tastefulserver/http.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpHandler : public ConnectionHandler
{
public:
    enum State
    {
        READ_REQUEST_LINE, READ_HEADER, READ_CONTENT, HANDLE_REQUEST, HANDLE_ERROR
    };

    typedef std::function<HttpResponse(const HttpRequest &)> RequestCallback;

    HttpHandler(const RequestCallback & callback);
    HttpHandler(const RequestCallback & callback, const RequestCallback & badRequestCallback);

    void receive(const QByteArray & data);

    void setBadRequestCallback(const RequestCallback & badRequestCallback);
    void uninstallBadRequestCallback();

private:
    bool readRequestLine();
    bool readHeader();
    bool readContent();
    bool handleRequest();
    bool handleError();

    RequestCallback m_callback;
    RequestCallback m_badRequestCallback;
    bool m_hasBadRequestCallback;
    ByteArrayStream m_buffer;
    HttpRequest m_request;
    State m_state;
};

} // namespace tastefulserver
