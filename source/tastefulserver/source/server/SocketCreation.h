#pragma once

#include <QAbstractSocket>
#include <QSslCertificate>
#include <QSslKey>

namespace tastefulserver {

class SocketCreation
{
public:
    SocketCreation(qintptr socketDescriptor);
    virtual ~SocketCreation();

    virtual QAbstractSocket * operator()() const = 0;
    virtual bool isTcp() const;
    virtual bool isSsl() const;
    virtual bool isUdp() const;

protected:
    qintptr m_socketDescriptor;
};

class UdpSocketCreation : public SocketCreation
{
public:
    UdpSocketCreation(qintptr socketDescriptor);
    virtual QAbstractSocket * operator()() const;
    virtual bool isUdp() const;
};

class TcpSocketCreation : public SocketCreation
{
public:
    TcpSocketCreation(qintptr socketDescriptor);
    virtual QAbstractSocket * operator()() const;
    virtual bool isTcp() const;
};

class SslSocketCreation : public TcpSocketCreation
{
public:
    SslSocketCreation(qintptr socketDescriptor, const QSslCertificate & certificate, const QSslKey & privateKey);
    virtual QAbstractSocket * operator()() const;
    virtual bool isSsl() const;

protected:
    QSslCertificate m_certificate;
    QSslKey m_privateKey;
};

} // namespace tastefulserver
