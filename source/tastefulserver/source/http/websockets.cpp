#include <tastefulserver/websockets.h>

#include <QCryptographicHash>

namespace tastefulserver {

QString hashWebSocketKey(const QString & key)
{
    return QCryptographicHash::hash((key + websocketMagicString).toUtf8(), QCryptographicHash::Sha1).toBase64();
}

HttpResponse websocketHandshake(const HttpRequest & request)
{
    HttpResponse response;

    response.clearContent();
    response.clearHeaders();

    response.setStatusCode(http::SwitchingProtocols);

    response.setHeader(http::Upgrade, "websocket");
    response.setHeader(http::Connection, http::Upgrade);
    response.setHeader(http::SecWebSocketAccept, hashWebSocketKey(request.getHeader(http::SecWebSocketKey).getValue()));

    //response.addHeader(request.getHeader(http::SecWebSocketProtocol));

    return response;
}

} // namespace tastefulserver
