#pragma once

#include <tastefulserver/tastefulserver_api.h>

#include <tastefulserver/AbstractSocket.h>
#include <tastefulserver/WebSocketFrame.h>
#include <tastefulserver/WebSocketFrameParser.h>
#include <tastefulserver/http.h>

namespace tastefulserver {

class WebSocketHandler;
class HttpSocket;

class TASTEFULSERVER_API WebSocket : public AbstractSocket
{
    Q_OBJECT
public:
    WebSocket(WebSocketHandler * handler);

    void upgrade(HttpSocket * socket, const HttpRequest & request);

    void performHandshake(const HttpRequest & request);

    void sendText(const QByteArray & text);
    void sendBinary(const QByteArray & binary);
    void sendPing();

    void closeConnection();
protected:
    WebSocketHandler * m_handler;
    WebSocketFrameParser m_parser;
    WebSocketFrame m_fragmentedMessage;
    bool m_inFragmentedMode;

    QAbstractSocket * createSocket(qintptr socketDescriptor);

    virtual void receiveData(const QByteArray & data) override;

    void sendFrame(const WebSocketFrame & frame);
    void sendPong();

    static const QString MagicString;
    static QString hashKey(const QString & key);
protected slots:
    void badFrame();
};

} // namespace tastefulserver
