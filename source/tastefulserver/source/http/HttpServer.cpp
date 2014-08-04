#include <tastefulserver/HttpServer.h>

#include "../server/SocketCreation.h"

namespace tastefulserver {

HttpServer::HttpServer(const HttpHandler::RequestCallback & callback)
    : m_callback(callback)
{
}

ConnectionHandler * HttpServer::createConnectionHandler(qintptr socketDescriptor) const
{
    HttpHandler * http = new HttpHandler(m_callback);

    http->setSocketCreator(new TcpSocketCreation(socketDescriptor));

    return http;
}

} // namespace tastefulserver
