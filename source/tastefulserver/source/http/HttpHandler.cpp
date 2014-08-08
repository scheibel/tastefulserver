#include <tastefulserver/HttpHandler.h>

#include <QStringList>

#include <tastefulserver/HttpHeader.h>

namespace tastefulserver {

HttpHandler::HttpHandler(const RequestCallback & callback)
    : m_callback(callback)
    , m_buffer(ByteArrayStream::forLinebreak(http::Linebreak))
{
    m_badRequestCallback = [](const HttpRequest & request) {
            HttpResponse response(request);

            response.setStatusCode(http::BadRequest);

            return response;
        };

    m_state = READ_REQUEST_LINE;
}

HttpHandler::HttpHandler(const RequestCallback & callback, const RequestCallback & badRequestCallback)
    : m_callback(callback)
    , m_badRequestCallback(badRequestCallback)
    , m_hasBadRequestCallback(true)
    , m_buffer(ByteArrayStream::forLinebreak(http::Linebreak))
{
    m_state = READ_REQUEST_LINE;
}

void HttpHandler::receive(const QByteArray & data)
{
    m_buffer.append(data);

    bool continueReading = true;

    while (continueReading)
    {
        switch (m_state)
        {
            case READ_REQUEST_LINE:
                continueReading = readRequestLine();
                break;
            case READ_HEADER:
                continueReading = readHeader();
                break;
            case READ_CONTENT:
                continueReading = readContent();
                break;
            case HANDLE_REQUEST:
                continueReading = handleRequest();
                break;
            case HANDLE_ERROR:
                continueReading = handleError();
                break;
        }
    }
}

void HttpHandler::setBadRequestCallback(const RequestCallback & badRequestCallback)
{
    m_badRequestCallback = badRequestCallback;
    m_hasBadRequestCallback = true;
}

void HttpHandler::uninstallBadRequestCallback()
{
    m_badRequestCallback = [](const HttpRequest & request) {
            HttpResponse response(request);

            response.setStatusCode(http::BadRequest);

            return response;
        };

    m_hasBadRequestCallback = false;
}

bool HttpHandler::readRequestLine()
{
    if (!m_buffer.canReadLine())
    {
        return false;
    }

    QString line = m_buffer.readLine();

    if (line.isEmpty())
    {
        return true;
    }

    QStringList parts = line.split(' ');
    if (parts.size()<3)
    {
        m_state = HANDLE_ERROR;

        return true;
    }
    HttpMethod method = HttpMethod::fromString(parts[0]);
    HttpVersion httpVersion = HttpVersion::fromString(parts[2]);

    if (method.isInvalid() || httpVersion.isInvalid())
    {
        m_state = HANDLE_ERROR;

        return true;
    }

    QString requestUri = parts[1];

    m_request = HttpRequest(method, requestUri, httpVersion, isSslConnection());
    m_request.setAddress(m_socket->peerAddress());
    m_request.setPort(m_socket->peerPort());

    m_state = READ_HEADER;

    return true;
}

bool HttpHandler::readHeader()
{
    if (!m_buffer.canReadLine())
    {
        return false;
    }

    QString line = m_buffer.readLine();
    if (line.isEmpty())
    {
        m_state = m_request.hasHeader(http::ContentLength) ? READ_CONTENT : HANDLE_REQUEST;

        return true;
    }
    else
    {
        int pos = line.indexOf(": ");
        if (pos<0)
        {
            m_request.markBad();

            return true;
        }

        QString fieldName = line.left(pos);
        QString fieldValue = line.mid(pos + 2);
        HttpHeader header(fieldName, fieldValue);
        m_request.parseHeader(header);
    }

    return true;
}

bool HttpHandler::readContent()
{
    int length = m_request.getContentLength();

    if (m_buffer.availableBytes()<length)
    {
        return false;
    }
    QByteArray content = m_buffer.read(length);
    m_request.parseContent(content);
    m_state = HANDLE_REQUEST;

    return true;
}

void HttpHandler::send(const HttpResponse & response)
{
    response.writeTo(*m_socket);
}

bool HttpHandler::handleRequest()
{
    HttpResponse response = m_hasBadRequestCallback && m_request.isBad() ? m_badRequestCallback(m_request) : m_callback(m_request);

    send(response);
    m_buffer.flush();
    m_state = READ_REQUEST_LINE;

    if (!response.isKeepAlive())
    {
        disconnect();
    }

    return false;
}

bool HttpHandler::handleError()
{
    m_request.markBad();

    HttpResponse response;
    response.setStatusCode(http::BadRequest);

    send(response);

    m_state = READ_REQUEST_LINE;

    return true;
}

} // namespace tastefulserver
