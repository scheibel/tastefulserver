#include <internal/SocketCreation>

using namespace internal;

SocketCreation::SocketCreation(int socketDescriptor) : socketDescriptor(socketDescriptor) {
}

bool SocketCreation::isTcp() {
	return false;
}

bool SocketCreation::isSsl() {
	return false;
}

bool SocketCreation::isUdp() {
	return false;
}


UdpSocketCreation::UdpSocketCreation(int socketDescriptor) : SocketCreation(socketDescriptor) {	
}

QAbstractSocket* UdpSocketCreation::operator()() {
	QUdpSocket* socket = new QUdpSocket();
	socket->setSocketDescriptor(socketDescriptor);
	return socket;
}

bool UdpSocketCreation::isUdp() {
	return true;
}


TcpSocketCreation::TcpSocketCreation(int socketDescriptor) : SocketCreation(socketDescriptor) {
}

QAbstractSocket* TcpSocketCreation::operator()() {
	QTcpSocket* socket = new QTcpSocket();
	socket->setSocketDescriptor(socketDescriptor);
	return socket;
}

bool TcpSocketCreation::isTcp() {
	return true;
}


SslSocketCreation::SslSocketCreation(int socketDescriptor, QSslCertificate certificate, QSslKey privateKey) : TcpSocketCreation(socketDescriptor), certificate(certificate), privateKey(privateKey) {
}

QAbstractSocket* SslSocketCreation::operator()() {
	QSslSocket* socket = new QSslSocket();
	socket->setSocketDescriptor(socketDescriptor);
	
	socket->setLocalCertificate(certificate);
	socket->setPrivateKey(privateKey);

	socket->startServerEncryption();
	
	return socket;
}

bool SslSocketCreation::isSsl() {
	return true;
}

