#include <tastefulserver/Connection.h>
#include <tastefulserver/AbstractSocket.h>

#include <QDebug>

namespace tastefulserver {

Connection::Connection(qintptr socketDescriptor)
: m_socketDescriptor(socketDescriptor)
, m_socket(nullptr)
{
}

Connection::~Connection()
{
    delete m_socket;
}

void Connection::setSocket(AbstractSocket * socket)
{
    if (m_socket)
    {
        disconnect(m_socket, &AbstractSocket::disconnected, this, &Connection::disconnected);
        m_socket->deleteLater();
        m_socket->setConnection(nullptr);
        m_socket->setParent(nullptr);
    }


    m_socket = socket;
    m_socket->setConnection(this);
    m_socket->setParent(this); // so that socket will be moved to the same thread as this
    connect(m_socket, &AbstractSocket::disconnected, this, &Connection::disconnected);
}

void Connection::startUp()
{
    m_socket->create(m_socketDescriptor);
}

void Connection::disconnected()
{
    finish();
}


} // namespace tastefulserver
