#include <HttpsServer>

using namespace internal;

HttpsServer::HttpsServer(QSslCertificate certificate, QSslKey privateKey, HttpDataHandler::RequestCallback callback) : HttpServer(callback), certificate(certificate), privateKey(privateKey) {
}

TcpDataHandler* HttpsServer::createDataHandler() {
	return new HttpDataHandler(callback, true);
}

Task* HttpsServer::createTask(int socketDescriptor) {
	return new SslConnectionHandler(socketDescriptor, certificate, privateKey, createDataHandler());
}