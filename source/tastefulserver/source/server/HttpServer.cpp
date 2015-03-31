#include <tastefulserver/HttpServer.h>

#include <tastefulserver/HttpSocket.h>

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
