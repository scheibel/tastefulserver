#pragma once

#include <QTcpServer>

#include <tastefulserver/tasteful-server_api.h>
#include <tastefulserver/Connection.h>

namespace tastefulserver {

class ThreadPool;

class TASTEFUL_SERVER_API TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TcpServer();
    virtual ~TcpServer();

    static void setNumThreads(int numThreads);

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

    virtual AbstractSocket * createSocket() = 0;

    static ThreadPool * s_threadPool;
    static int s_serverCount;
};

} // namespace tastefulserver
