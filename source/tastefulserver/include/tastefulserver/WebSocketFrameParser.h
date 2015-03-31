#pragma once

#include <tastefulserver/tasteful-server_api.h>

#include <tastefulserver/ByteStream.h>
#include <tastefulserver/WebSocketFrame.h>

#include <QObject>
#include <QQueue>
#include <QByteArray>
#include <array>

namespace tastefulserver {

class TASTEFUL_SERVER_API WebSocketFrameParser : public QObject
{
    Q_OBJECT
public:
    WebSocketFrameParser();

    void addData(const QByteArray & data);

    bool hasReadyFrames() const;
    WebSocketFrame popReadyFrame();

signals:
    void badFrame();

protected:
    enum class ParseState
    {
        Header,
        LengthMask,
        ExtendedLength,
        Mask,
        Content,
        Finish,
        Error,
        Interrupted
    };

    ByteStream m_byteStream;
    WebSocketFrame m_currentFrame;
    ParseState m_state;
    ParseState m_interruptedState;
    QQueue<WebSocketFrame> m_readyFrames;

    void parse();

    ParseState dispatch(ParseState state);

    ParseState parseFrameHeader();
    ParseState parseLengthMask();
    ParseState parseExtendedLength();
    ParseState parseMask();
    ParseState parseContent();
    ParseState finishFrame();
    ParseState handleError();

    void pushFrame();

    qint64 length;
    WebSocketFrame::LengthMask lengthMask;
    std::array<char, 4> mask;
};

} // namespace tastefulserver
