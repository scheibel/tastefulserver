#include <QCoreApplication>
#include <HttpServer>

int main(int argc, char** argv) {
	QCoreApplication app(argc, argv);
	
	HttpServer server([](HttpRequest& request) {
		HttpResponse response;
		
		response.setStatusCode(http::OK);
		response.setContent(request.toByteArray());
		
		return response;
	});
	
	server.listen(QHostAddress::Any, 8080);
	
	return app.exec();
}
