#include <internal/SocketCreation>

using namespace internal;

SocketCreation::SocketCreation(int socketDescriptor) : socketDescriptor(socketDescriptor) {
}

bool SocketCreation::isTcp() const {
	return false;
}

bool SocketCreation::isSsl() const {
	return false;
}

bool SocketCreation::isUdp() const {
	return false;
}


UdpSocketCreation::UdpSocketCreation(int socketDescriptor) : SocketCreation(socketDescriptor) {	
}

QAbstractSocket* UdpSocketCreation::operator()() const {
	QUdpSocket* socket = new QUdpSocket();
	socket->setSocketDescriptor(socketDescriptor);
	return socket;
}

bool UdpSocketCreation::isUdp() const {
	return true;
}


TcpSocketCreation::TcpSocketCreation(int socketDescriptor) : SocketCreation(socketDescriptor) {
}

QAbstractSocket* TcpSocketCreation::operator()() const {
	QTcpSocket* socket = new QTcpSocket();
	socket->setSocketDescriptor(socketDescriptor);
	return socket;
}

bool TcpSocketCreation::isTcp() const {
	return true;
}


SslSocketCreation::SslSocketCreation(int socketDescriptor, const QSslCertificate& certificate, const QSslKey& privateKey) : TcpSocketCreation(socketDescriptor), certificate(certificate), privateKey(privateKey) {
}

QAbstractSocket* SslSocketCreation::operator()() const {
	QSslSocket* socket = new QSslSocket();
	socket->setSocketDescriptor(socketDescriptor);
	
	socket->setLocalCertificate(certificate);
	socket->setPrivateKey(privateKey);

	socket->startServerEncryption();
	
	return socket;
}

bool SslSocketCreation::isSsl() const {
	return true;
}

