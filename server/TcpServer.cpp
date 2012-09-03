#include <TcpServer>

ThreadPool TcpServer::threadPool;

TcpServer::TcpServer() {
	if (!threadPool.isStarted()) {
		threadPool.start();
	}
}

void TcpServer::setNumThreads(int numThreads) {
	threadPool.setNumThreads(numThreads);
}

void TcpServer::incomingConnection(int socketDescriptor) {
	threadPool.addTask(createTask(socketDescriptor));
}

Task* TcpServer::createTask(int socketDescriptor) {
	return new TcpConnectionHandler(socketDescriptor, createDataHandler());
}
