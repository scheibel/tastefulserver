#include <tastefulserver/HttpCallbackServer.h>

namespace tastefulserver {

HttpCallbackServer::HttpCallbackServer(const RequestCallback & callback, int numThreads)
: HttpServer(numThreads)
, m_callback(callback)
{
}

HttpCallbackServer::~HttpCallbackServer()
{
}

void HttpCallbackServer::handleRequest(HttpSocket * socket, const HttpRequest & request)
{
    socket->send(m_callback(request));
}

} // namespace tastefulserver
