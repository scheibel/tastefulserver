#include <tasteful-server/HttpSocket.h>
#include <tasteful-server/Connection.h>
#include <tasteful-server/HttpSocketHandler.h>

#include <QTcpSocket>

namespace tastefulserver {

HttpSocket::HttpSocket(HttpSocketHandler * handler)
: m_handler(handler)
{
    connect(&m_parser, &HttpRequestParser::badRequest, this, &HttpSocket::badRequest);
}

QAbstractSocket * HttpSocket::createSocket(qintptr socketDescriptor)
{
    QTcpSocket * socket = new QTcpSocket();

    socket->setSocketDescriptor(socketDescriptor);

    return socket;
}

void HttpSocket::send(const HttpResponse & response)
{
    response.writeTo(*m_socket);

    if (!response.isKeepAlive())
    {
        disconnect();
    }
}

void HttpSocket::disconnect()
{
    AbstractSocket::disconnect();
}

void HttpSocket::receiveData(const QByteArray & data)
{
    m_parser.addData(data);

    while (m_parser.hasReadyRequests())
    {
        HttpRequest request = m_parser.popReadyRequest();
        addConnectionInfo(request);

        if (request.hasHeader(http::Upgrade))
        {
            if (m_handler->handleUpgrade(this, request))
                break;
        }

        m_handler->handleRequest(this, request);
    }
}

void HttpSocket::addConnectionInfo(HttpRequest & request)
{
    request.setAddress(m_socket->peerAddress());
    request.setPort(m_socket->peerPort());
}

void HttpSocket::badRequest()
{
    m_handler->handleBadRequest(this);
}

} // namespace tastefulserver
