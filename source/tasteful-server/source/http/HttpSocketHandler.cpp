#include <tasteful-server/HttpSocketHandler.h>

#include <tasteful-server/HttpSocket.h>
#include <tasteful-server/http.h>

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
