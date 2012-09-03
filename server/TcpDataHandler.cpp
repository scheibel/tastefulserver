#include <TcpDataHandler>

#include <QDebug>


void TcpDataHandler::send(QByteArray data) {
	emit(sendDataReady(data));
}

void TcpDataHandler::disconnect() {
	emit(closeConnection());
}

void TcpDataHandler::disconnected() {
}
