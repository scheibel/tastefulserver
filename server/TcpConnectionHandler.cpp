#include <internal/TcpConnectionHandler>

using namespace internal;

TcpConnectionHandler::TcpConnectionHandler(unsigned socketDescriptor, TcpDataHandler* handler) : socketDescriptor(socketDescriptor), socket(nullptr), handler(handler) {	
}

void TcpConnectionHandler::startUp() {
	socket = createSocket();
	
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
	QObject::connect(handler, SIGNAL(sendDataReady(QByteArray)), this, SLOT(writeData(QByteArray)));
	QObject::connect(handler, SIGNAL(closeConnection()), this, SLOT(disconnect()));
}

QTcpSocket* TcpConnectionHandler::createSocket() {
	socket = new QTcpSocket();
	socket->setSocketDescriptor(socketDescriptor);
	return socket;
}

TcpConnectionHandler::~TcpConnectionHandler() {
	delete socket;
	delete handler;
}

void TcpConnectionHandler::disconnected() {
	handler->disconnected();
	finish();
}

void TcpConnectionHandler::readyRead() {
	handler->receive(socket->readAll());
}

void TcpConnectionHandler::writeData(QByteArray data) {
	socket->write(data);
}

void TcpConnectionHandler::error(QAbstractSocket::SocketError e) {
	if (e!=QAbstractSocket::RemoteHostClosedError) {
		qDebug() << "Socket error: " << socket->errorString();
	}
}

void TcpConnectionHandler::disconnect() {
	socket->disconnectFromHost();
}


SslConnectionHandler::SslConnectionHandler(unsigned socketDescriptor, QSslCertificate certificate, QSslKey privateKey, TcpDataHandler* handler)
 : TcpConnectionHandler(socketDescriptor, handler), certificate(certificate), privateKey(privateKey) {	
}

QTcpSocket* SslConnectionHandler::createSocket() {
	QSslSocket* socket = new QSslSocket();
	socket->setSocketDescriptor(socketDescriptor);
	
	socket->setLocalCertificate(certificate);
	socket->setPrivateKey(privateKey);

	socket->startServerEncryption();
	
	return socket;
}

