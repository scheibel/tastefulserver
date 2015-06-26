#include <tastefulserver/WebSocketHandler.h>

namespace tastefulserver {

void WebSocketHandler::connectionEstablished(const HttpRequest & /*request*/, WebSocket * /*socket*/)
{
}

void WebSocketHandler::connectionClosed(const HttpRequest & /*request*/, WebSocket * /*socket*/)
{
}

} // namespace tastefulserver
