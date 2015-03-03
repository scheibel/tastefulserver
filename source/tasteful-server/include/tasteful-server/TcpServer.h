#pragma once

#include <QTcpServer>

#include <tasteful-server/tasteful-server_api.h>
#include <tasteful-server/Connection.h>

namespace tastefulserver {

class ThreadPool;

class TASTEFUL_SERVER_API TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TcpServer(int numThreads = 1);
    virtual ~TcpServer();

    static void setNumThreads(int numThreads);

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

    virtual AbstractSocket * createSocket() = 0;

    static ThreadPool * s_threadPool;
    static int s_serverCount;
};

} // namespace tastefulserver
