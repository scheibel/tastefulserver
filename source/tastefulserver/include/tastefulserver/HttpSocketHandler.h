#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/HttpSocket.h>
#include <tastefulserver/http.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpSocketHandler
{
public:
    virtual void handleRequest(HttpSocket * socket, const HttpRequest & request) = 0;
    virtual void handleBadRequest(HttpSocket * socket);
    virtual bool handleUpgrade(HttpSocket * socket, const HttpRequest & request);
};

} // namespace tastefulserver
