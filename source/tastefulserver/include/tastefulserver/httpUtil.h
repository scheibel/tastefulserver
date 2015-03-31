#pragma once

#include <tastefulserver/tastefulserver_api.h>

class QString;
class QDateTime;

namespace tastefulserver {

namespace http {
TASTEFULSERVER_API QString reason(const unsigned statusCode);
TASTEFULSERVER_API QString dateString(const QDateTime & dateTime);
} // namespace http

} // namespace tastefulserver
