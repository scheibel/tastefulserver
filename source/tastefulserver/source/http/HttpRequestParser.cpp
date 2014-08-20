#include <tastefulserver/HttpRequestParser.h>

#include <QStringList>

#include <tastefulserver/HttpHeader.h>

namespace tastefulserver {

HttpRequestParser::HttpRequestParser()
: m_byteStream(ByteStream::forLinebreak(http::Linebreak))
, m_state(ParseState::RequestLine)
, m_interruptedState(ParseState::RequestLine)
{
}

HttpRequestParser::~HttpRequestParser()
{
}

void HttpRequestParser::addData(const QByteArray & data)
{
    m_byteStream.append(data);

    parse();
}

bool HttpRequestParser::hasReadyRequests() const
{
    return !m_readyRequests.empty();
}

HttpRequest HttpRequestParser::popReadyRequest()
{
    HttpRequest request = m_readyRequests.front();
    m_readyRequests.pop_front();

    return request;
}

void HttpRequestParser::pushRequest()
{
    m_readyRequests.push_back(m_currentRequest);
}

void HttpRequestParser::parse()
{
    while (true)
    {
        ParseState nextState = dispatch(m_state);

        if (nextState == ParseState::Interrupted)
        {
            m_interruptedState = m_state;
            break;
        }

        m_state = nextState;
    }
}

HttpRequestParser::ParseState HttpRequestParser::dispatch(ParseState state)
{
    switch (state)
    {
        case ParseState::RequestLine:
            return parseRequestLine();
        case ParseState::HeaderLine:
            return parseHeaderLine();
        case ParseState::Content:
            return parseContent();
        case ParseState::Finish:
            return finishRequest();
        case ParseState::Error:
            return handleError();
        case ParseState::Interrupted:
            return m_interruptedState;
        default: // for compiler, cannot happen
            return ParseState::Error;
    }
}

HttpRequestParser::ParseState HttpRequestParser::parseRequestLine()
{
    if (!m_byteStream.canReadLine())
    {
        return ParseState::Interrupted;
    }

    QString line = m_byteStream.readLine();

    if (line.isEmpty())
    {
        return ParseState::RequestLine;
    }

    QStringList parts = line.split(' ');
    if (parts.size()<3)
    {
        return ParseState::Error;
    }

    HttpMethod method = HttpMethod::fromString(parts[0]);
    HttpVersion httpVersion = HttpVersion::fromString(parts[2]);

    if (method.isInvalid() || httpVersion.isInvalid())
    {
        return ParseState::Error;
    }

    QString requestUri = parts[1];

    m_currentRequest = HttpRequest(method, requestUri, httpVersion);

    return ParseState::HeaderLine;
}

HttpRequestParser::ParseState HttpRequestParser::parseHeaderLine()
{
    if (!m_byteStream.canReadLine())
    {
        return ParseState::Interrupted;
    }

    QString line = m_byteStream.readLine();

    if (line.isEmpty())
    {
        return m_currentRequest.hasHeader(http::ContentLength) ? ParseState::Content : ParseState::Finish;
    }

    int pos = line.indexOf(": ");
    if (pos<0)
    {
        return ParseState::Error;
    }

    QString fieldName = line.left(pos);
    QString fieldValue = line.mid(pos + 2);

    m_currentRequest.addHeader(HttpHeader(fieldName, fieldValue));

    return ParseState::HeaderLine;
}

HttpRequestParser::ParseState HttpRequestParser::parseContent()
{
    int length = m_currentRequest.getContentLength();

    if (m_byteStream.availableBytes()<length)
    {
        return ParseState::Interrupted;
    }

    m_currentRequest.setContent(m_byteStream.read(length));

    return ParseState::Finish;
}

HttpRequestParser::ParseState HttpRequestParser::finishRequest()
{
    m_currentRequest.finalize();

    pushRequest();

    m_byteStream.flush();

    return ParseState::RequestLine;
}

HttpRequestParser::ParseState HttpRequestParser::handleError()
{
    emit(badRequest());

    return ParseState::RequestLine;
}

} // namespace tastefulserver
