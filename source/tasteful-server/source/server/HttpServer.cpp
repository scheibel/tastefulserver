#include <tasteful-server/HttpServer.h>

#include <tasteful-server/HttpSocket.h>

namespace tastefulserver {

HttpServer::HttpServer(int numThreads)
: TcpServer(numThreads)
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
