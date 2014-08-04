#include "SocketCreation.h"

#include <QUdpSocket>
#include <QTcpSocket>
#include <QSslSocket>

namespace tastefulserver {

SocketCreation::SocketCreation(qintptr socketDescriptor)
    : m_socketDescriptor(socketDescriptor)
{
}

SocketCreation::~SocketCreation()
{
}

bool SocketCreation::isTcp() const
{
    return false;
}

bool SocketCreation::isSsl() const
{
    return false;
}

bool SocketCreation::isUdp() const
{
    return false;
}

UdpSocketCreation::UdpSocketCreation(qintptr socketDescriptor)
    : SocketCreation(socketDescriptor)
{
}

QAbstractSocket * UdpSocketCreation::operator()() const
{
    QUdpSocket * socket = new QUdpSocket();

    socket->setSocketDescriptor(m_socketDescriptor);

    return socket;
}

bool UdpSocketCreation::isUdp() const
{
    return true;
}

TcpSocketCreation::TcpSocketCreation(qintptr socketDescriptor)
    : SocketCreation(socketDescriptor)
{
}

QAbstractSocket * TcpSocketCreation::operator()() const
{
    QTcpSocket * socket = new QTcpSocket();

    socket->setSocketDescriptor(m_socketDescriptor);

    return socket;
}

bool TcpSocketCreation::isTcp() const
{
    return true;
}

SslSocketCreation::SslSocketCreation(qintptr socketDescriptor, const QSslCertificate & certificate, const QSslKey & privateKey)
    : TcpSocketCreation(socketDescriptor)
    , m_certificate(certificate)
    , m_privateKey(privateKey)
{
}

QAbstractSocket * SslSocketCreation::operator()() const
{
    QSslSocket * socket = new QSslSocket();

    socket->setSocketDescriptor(m_socketDescriptor);

    socket->setLocalCertificate(m_certificate);
    socket->setPrivateKey(m_privateKey);

    socket->startServerEncryption();

    return socket;
}

bool SslSocketCreation::isSsl() const
{
    return true;
}

} // namespace tastefulserver
