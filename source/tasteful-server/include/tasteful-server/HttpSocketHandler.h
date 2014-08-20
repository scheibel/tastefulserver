#pragma once

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/HttpSocket.h>
#include <tasteful-server/http.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API HttpSocketHandler
{
public:
    virtual void handleRequest(HttpSocket * socket, const HttpRequest & request) = 0;
    virtual void handleBadRequest(HttpSocket * socket);
    virtual bool handleUpgrade(HttpSocket * socket, const HttpRequest & request);
};

} // namespace tastefulserver
