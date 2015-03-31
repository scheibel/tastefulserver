#include <tastefulserver/WebSocket.h>

#include <tastefulserver/WebSocketHandler.h>
#include <tastefulserver/HttpSocket.h>

#include <QCryptographicHash>
#include <QTcpSocket>
#include <QAbstractSocket>

namespace tastefulserver {

const QString WebSocket::MagicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

WebSocket::WebSocket(WebSocketHandler * handler)
: m_handler(handler)
, m_inFragmentedMode(false)
{
    connect(&m_parser, &WebSocketFrameParser::badFrame, this, &WebSocket::badFrame);
}

QAbstractSocket * WebSocket::createSocket(qintptr socketDescriptor)
{
    QTcpSocket * socket = new QTcpSocket();

    socket->setSocketDescriptor(socketDescriptor);

    return socket;
}

QString WebSocket::hashKey(const QString & key)
{
    return QCryptographicHash::hash((key + MagicString).toLatin1(), QCryptographicHash::Sha1).toBase64();
}

void WebSocket::performHandshake(const HttpRequest & request)
{
    HttpResponse response(http::SwitchingProtocols);

    response.setHeader(http::Upgrade, "websocket");
    response.setHeader(http::Connection, http::Upgrade);
    response.setHeader(http::SecWebSocketAccept, hashKey(request.getHeader(http::SecWebSocketKey).getValue()));
    response.addHeader(request.getHeader(http::SecWebSocketVersion));

    QStringList protocols = request.getHeader(http::SecWebSocketProtocol).getValue().split(",", QString::SkipEmptyParts);
    if (!protocols.empty())
    {
        response.setHeader(http::SecWebSocketProtocol, protocols.first().trimmed());
    }

    response.writeTo(*m_socket);

    m_handler->connectionEstablished(this);

    connect(m_socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
}

void WebSocket::upgrade(HttpSocket * socket, const HttpRequest & request)
{
    takeOverFrom(socket);
    performHandshake(request);
}

void WebSocket::receiveData(const QByteArray & data)
{
    m_parser.addData(data);

    while (m_parser.hasReadyFrames())
    {
        WebSocketFrame frame = m_parser.popReadyFrame();

        if (!frame.isFinal())
        {
            if (frame.isControlFrame())
            {
                disconnect();
                break;
            }

            if (!m_inFragmentedMode)
            {
                m_inFragmentedMode = true;
                m_fragmentedMessage = frame;
            }
            else
            {
                if (!frame.isContinuationFrame())
                {
                    disconnect();
                    break;
                }

                m_fragmentedMessage.append(frame.getContent());
            }

            continue;
        }

        if (frame.isContinuationFrame())
        {
            if (!m_inFragmentedMode)
            {
                disconnect();
                break;
            }

            m_fragmentedMessage.append(frame.getContent());
            m_inFragmentedMode = false;

            frame = m_fragmentedMessage;
        }

        switch (frame.getOpCode())
        {
            case WebSocketFrame::OpCode::Text:
                m_handler->handleText(this, frame.getContent());
                break;
            case WebSocketFrame::OpCode::Binary:
                m_handler->handleBinary(this, frame.getContent());
                break;
            case WebSocketFrame::OpCode::Ping:
                sendPong();
                break;
            case WebSocketFrame::OpCode::Pong:
                break;
            case WebSocketFrame::OpCode::ConnectionClose:
                sendFrame(WebSocketFrame(WebSocketFrame::OpCode::ConnectionClose));
                disconnect();
                break;
            default:
                break;
        }
    }
}

void WebSocket::badFrame()
{
    disconnect();
}

void WebSocket::sendFrame(const WebSocketFrame & frame)
{
    frame.writeTo(*m_socket);
}

void WebSocket::sendText(const QByteArray & text)
{
    sendFrame(WebSocketFrame(WebSocketFrame::OpCode::Text, text));
}

void WebSocket::sendBinary(const QByteArray & binary)
{
    sendFrame(WebSocketFrame(WebSocketFrame::OpCode::Binary, binary));
}

void WebSocket::sendPing()
{
    sendFrame(WebSocketFrame(WebSocketFrame::OpCode::Ping));
}

void WebSocket::sendPong()
{
    sendFrame(WebSocketFrame(WebSocketFrame::OpCode::Pong));
}

void WebSocket::closeConnection()
{
    sendFrame(WebSocketFrame(WebSocketFrame::OpCode::ConnectionClose));
    disconnect();
}

void WebSocket::connectionClosed()
{
    m_handler->connectionClosed(this);
}

} // namespace tastefulserver
