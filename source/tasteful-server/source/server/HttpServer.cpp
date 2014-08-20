#include <tasteful-server/HttpServer.h>

#include <tasteful-server/HttpSocket.h>

namespace tastefulserver {

HttpServer::HttpServer()
{
}

HttpServer::~HttpServer()
{
}

AbstractSocket * HttpServer::createSocket()
{
    return new HttpSocket(this);
}

} // namespace tastefulserver
