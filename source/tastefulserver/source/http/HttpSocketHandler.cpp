#include <tastefulserver/HttpSocketHandler.h>

#include <tastefulserver/HttpSocket.h>
#include <tastefulserver/http.h>

namespace tastefulserver {


void HttpSocketHandler::handleBadRequest(HttpSocket * socket)
{
    socket->send(HttpResponse(http::BadRequest));
}

bool HttpSocketHandler::handleUpgrade(HttpSocket * socket, const HttpRequest &)
{
    socket->send(HttpResponse(http::NotImplemented));
    return false;
}

} // namespace tastefulserver
