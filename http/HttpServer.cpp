#include <HttpServer>

HttpServer::HttpServer(HttpDataHandler::RequestCallback callback) : callback(callback) {
}

TcpDataHandler* HttpServer::createDataHandler() {
	return new HttpDataHandler(callback, false);
}