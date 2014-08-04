#pragma once

#include <functional>

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/TcpServer.h>
#include <tastefulserver/HttpHandler.h>

namespace tastefulserver {

class TASTEFULSERVER_API HttpServer : public TcpServer
{
public:
    HttpServer(const HttpHandler::RequestCallback & callback);

protected:
    HttpHandler::RequestCallback m_callback;
    virtual ConnectionHandler* createConnectionHandler(qintptr socketDescriptor) const;
};

} // namespace tastefulserver
