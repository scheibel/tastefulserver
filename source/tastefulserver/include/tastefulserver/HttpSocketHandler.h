#pragma once

#include <tastefulserver/tasteful-server_api.h>

#include <tastefulserver/HttpSocket.h>
#include <tastefulserver/http.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API HttpSocketHandler
{
public:
    virtual void handleRequest(HttpSocket * socket, const HttpRequest & request) = 0;
    virtual void handleBadRequest(HttpSocket * socket);
    virtual bool handleUpgrade(HttpSocket * socket, const HttpRequest & request);
};

} // namespace tastefulserver
