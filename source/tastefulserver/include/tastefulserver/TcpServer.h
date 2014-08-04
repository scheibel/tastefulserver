#pragma once

#include <QTcpServer>

#include <tastefulserver/tastefulserver_api.h>

namespace tastefulserver {

class ThreadPool;
class ConnectionHandler;

class TASTEFULSERVER_API TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TcpServer();
    virtual ~TcpServer();

    static void setNumThreads(int numThreads);

protected:
    virtual void incomingConnection(qintptr socketDescriptor);
    virtual ConnectionHandler* createConnectionHandler(qintptr socketDescriptor) const = 0;

    static ThreadPool * s_threadPool;
    static int s_serverCount;
};

} // namespace tastefulserver
