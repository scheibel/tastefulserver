#include <tasteful-server/WebSocketFrameParser.h>

#include <QtEndian>
#include <QDebug>
#include <cassert>

namespace tastefulserver {

WebSocketFrameParser::WebSocketFrameParser()
: m_state(ParseState::Header)
, m_interruptedState(ParseState::Header)
{
}

void WebSocketFrameParser::addData(const QByteArray & data)
{
    m_byteStream.append(data);

    parse();
}

bool WebSocketFrameParser::hasReadyFrames() const
{
    return !m_readyFrames.empty();
}

WebSocketFrame WebSocketFrameParser::popReadyFrame()
{
    WebSocketFrame frame = m_readyFrames.back();
    m_readyFrames.pop_back();
    return frame;
}

void WebSocketFrameParser::pushFrame()
{
    m_readyFrames.push_back(m_currentFrame);
}

void WebSocketFrameParser::parse()
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

WebSocketFrameParser::ParseState WebSocketFrameParser::dispatch(ParseState state)
{
    switch (state)
    {
        case ParseState::Header:
            return parseFrameHeader();
        case ParseState::LengthMask:
            return parseLengthMask();
        case ParseState::ExtendedLength:
            return parseExtendedLength();
        case ParseState::Mask:
            return parseMask();
        case ParseState::Content:
            return parseContent();
        case ParseState::Finish:
            return finishFrame();
        case ParseState::Error:
            return handleError();
        default:
            return ParseState::Error;
    }
}

WebSocketFrameParser::ParseState WebSocketFrameParser::parseFrameHeader()
{
    if (m_byteStream.atEnd())
        return ParseState::Interrupted;

    length = 0;
    WebSocketFrame::Header header;
    header.raw = m_byteStream.readByte();

    m_currentFrame = WebSocketFrame(header);

    return ParseState::LengthMask;
}

WebSocketFrameParser::ParseState WebSocketFrameParser::parseLengthMask()
{
    if (m_byteStream.atEnd())
        return ParseState::Interrupted;

    lengthMask.raw = m_byteStream.readByte();

    if (lengthMask.data.len < WebSocketFrame::Length2Bytes)
    {
        length = lengthMask.data.len;

        return ParseState::Mask;
    }
    else
    {
        return ParseState::ExtendedLength;
    }
}

WebSocketFrameParser::ParseState WebSocketFrameParser::parseExtendedLength()
{
    if (lengthMask.data.len == WebSocketFrame::Length2Bytes)
    {
        if (m_byteStream.availableBytes() < 2)
        {
            return ParseState::Interrupted;
        }

        qint16 length2Bytes = *reinterpret_cast<qint16*>(m_byteStream.read(2).data());

        length = qFromBigEndian(length2Bytes);
    }
    else if (lengthMask.data.len == WebSocketFrame::Length4Bytes)
    {
        if (m_byteStream.availableBytes() < 4)
        {
            return ParseState::Interrupted;
        }

        qint64 length4Bytes = *reinterpret_cast<qint64*>(m_byteStream.read(4).data());

        length = qFromBigEndian(length4Bytes);
    }
    else
    {
        return ParseState::Error;
    }

    return ParseState::Mask;
}

WebSocketFrameParser::ParseState WebSocketFrameParser::parseMask()
{
    if (lengthMask.data.mask == 0)
    {
        return ParseState::Content;
    }

    if (m_byteStream.availableBytes() < 4)
    {
        return ParseState::Interrupted;
    }

    QByteArray raw_mask = m_byteStream.read(4);

    mask = *reinterpret_cast<decltype(mask)*>(raw_mask.data());
    m_currentFrame.setMask(mask);

    return ParseState::Content;
}

WebSocketFrameParser::ParseState WebSocketFrameParser::parseContent()
{
    if (m_byteStream.availableBytes() < length)
    {
        return ParseState::Interrupted;
    }

    QByteArray content = m_byteStream.read(length);

    if (lengthMask.data.mask == 1)
    {
        for (int i = 0; i < content.length(); ++i)
        {
            content[i] = content[i] ^ mask[i % 4];
        }
    }

    m_currentFrame.setContent(content);

    return ParseState::Finish;
}

WebSocketFrameParser::ParseState WebSocketFrameParser::finishFrame()
{
    pushFrame();

    m_byteStream.flush();

    return ParseState::Header;
}

WebSocketFrameParser::ParseState WebSocketFrameParser::handleError()
{
    emit(badFrame());

    return ParseState::Header;
}

} // namespace tastefulserver
