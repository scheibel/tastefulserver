#pragma once

#include <tasteful-server/tasteful-server_api.h>

class QString;
class QDateTime;

namespace tastefulserver {

namespace http {
TASTEFUL_SERVER_API QString reason(const unsigned statusCode);
TASTEFUL_SERVER_API QString dateString(const QDateTime & dateTime);
} // namespace http

} // namespace tastefulserver
