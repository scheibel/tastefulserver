#pragma once

namespace tastefulserver {

namespace http {
const unsigned Continue = 100;
const unsigned SwitchingProtocols = 101;
const unsigned OK = 200;
const unsigned Created = 201;
const unsigned Accepted = 202;
const unsigned NonAuthoritativeInformation = 203;
const unsigned NoContent = 204;
const unsigned ResetContent = 205;
const unsigned PartialContent = 206;
const unsigned MultipleChoices = 300;
const unsigned MovedPermanently = 301;
const unsigned Found = 302;
const unsigned SeeOther = 303;
const unsigned NotModified = 304;
const unsigned UseProxy = 305;
const unsigned TemporaryRedirect = 307;
const unsigned BadRequest = 400;
const unsigned Unauthorized = 401;
const unsigned PaymentRequired = 402;
const unsigned Forbidden = 403;
const unsigned NotFound = 404;
const unsigned MethodNotAllowed = 405;
const unsigned NotAcceptable = 406;
const unsigned ProxyAuthenticationRequired = 407;
const unsigned RequestTimeout = 408;
const unsigned Conflict = 409;
const unsigned Gone = 410;
const unsigned LengthRequired = 411;
const unsigned PreconditionFailed = 412;
const unsigned RequestEntityTooLarge = 413;
const unsigned RequestURITooLarge = 414;
const unsigned UnsupportedMediaType = 415;
const unsigned RequestedRangeNotSatisfiable = 416;
const unsigned ExpectationFailed = 417;
const unsigned InternalServerError = 500;
const unsigned NotImplemented = 501;
const unsigned BadGateway = 502;
const unsigned ServiceUnavailable = 503;
const unsigned GatewayTimeout = 504;
const unsigned HTTPVersionNotSupported = 505;
}

} // namespace tastefulserver
