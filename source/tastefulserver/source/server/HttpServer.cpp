#include <tastefulserver/HttpServer.h>

#include <tastefulserver/HttpSocket.h>

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
