#include <tastefulserver/TcpServer.h>
#include "../core/ThreadPool.h"
#include <tastefulserver/ConnectionHandler.h>

namespace tastefulserver {

ThreadPool * TcpServer::s_threadPool = new ThreadPool();
int TcpServer::s_serverCount = 0;
TcpServer::TcpServer()
{
    s_serverCount++;
    if (!s_threadPool->isStarted())
    {
        s_threadPool->start();
    }
}

TcpServer::~TcpServer()
{
    s_serverCount--;
    if (s_serverCount<=0)
    {
        s_threadPool->stop();
    }
}

void TcpServer::setNumThreads(int numThreads)
{
    s_threadPool->setNumThreads(numThreads);
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    s_threadPool->addTask(createConnectionHandler(socketDescriptor));
}

} // namespace tastefulserver
