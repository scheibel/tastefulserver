#pragma once

#include <QAbstractSocket>

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/Task.h>

namespace tastefulserver {

class SocketCreation;

class TASTEFULSERVER_API ConnectionHandler : public Task
{
    Q_OBJECT

public:
    ConnectionHandler();
    ConnectionHandler(SocketCreation * socketCreation);
    ~ConnectionHandler();

    void setSocketCreator(SocketCreation * socketCreation);

    void startUp();

protected:
    QAbstractSocket * m_socket;

private:
    SocketCreation * m_socketCreation;
    void createSocket();

private slots:
    void disconnected();
    void readyRead();
    void error(QAbstractSocket::SocketError e);

protected:
    QAbstractSocket & socket();

    bool isUdpConnection() const;
    bool isTcpConnection() const;
    bool isSslConnection() const;

    void send(const QByteArray & data);
    void disconnect();

    virtual void onDisconnect();
    virtual void onError(QAbstractSocket::SocketError e);
    virtual void receive(const QByteArray & data) = 0;
};

} // namespace tastefulserver
