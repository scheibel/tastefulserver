#pragma once

#include <QAbstractSocket>

#include <tasteful-server/tasteful-server_api.h>

#include <tasteful-server/Task.h>

namespace tastefulserver {

class AbstractSocket;

class TASTEFUL_SERVER_API Connection : public Task
{
    Q_OBJECT

public:
    Connection(qintptr socketDescriptor);
    ~Connection();

    void setSocket(AbstractSocket * socket);

    void startUp();

protected:
    qintptr m_socketDescriptor;
    AbstractSocket * m_socket;

    void createSocket();

private slots:
    void disconnected();
};

} // namespace tastefulserver
