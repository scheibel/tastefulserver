#pragma once

#include <QObject>
#include <QAbstractSocket>

#include <tastefulserver/tasteful-server_api.h>

namespace tastefulserver {

class Connection;

class TASTEFUL_SERVER_API AbstractSocket : public QObject
{
    friend class Connection;

    Q_OBJECT

public:
    AbstractSocket();
    virtual ~AbstractSocket();

    Connection * connection();

    void create(qintptr socketDescriptor);

    void takeOverFrom(AbstractSocket * socket);

signals:
    void disconnected();

private slots:
    void socketDisconnected();
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError e);

protected:
    Connection * m_connection;
    QAbstractSocket * m_socket;

    void connectSocket();
    void disconnectSocket();

    void setConnection(Connection * connection);

    void disconnect();

    virtual void onDisconnect();
    virtual void onError(QAbstractSocket::SocketError e);
    virtual void receiveData(const QByteArray & data) = 0;

    virtual QAbstractSocket * createSocket(qintptr socketDescriptor) = 0;
};

} // namespace tastefulserver
