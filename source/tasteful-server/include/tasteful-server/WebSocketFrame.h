#pragma once

#include <tasteful-server/tasteful-server_api.h>

#include <QByteArray>
#include <QIODevice>

namespace tastefulserver {

class TASTEFUL_SERVER_API WebSocketFrame
{
public:
    union Header
    {
        struct {
            #ifdef LITTLE_ENDIAN
            unsigned int opcode : 4;
            unsigned int reserved3 : 1;
            unsigned int reserved2 : 1;
            unsigned int reserved1 : 1;
            unsigned int fin : 1;
            #else
            unsigned int fin : 1;
            unsigned int reserved1 : 1;
            unsigned int reserved2 : 1;
            unsigned int reserved3 : 1;
            unsigned int opcode : 4;
            #endif
        } data;
        char raw;
    };

    union LengthMask {
        struct {
            #ifdef LITTLE_ENDIAN
            unsigned int len : 7;
            unsigned int mask : 1;
            #else
            unsigned int mask : 1;
            unsigned int len : 7;
            #endif
        } data;
        char raw;
    };

    static const unsigned char Length2Bytes;
    static const unsigned char Length4Bytes;

    enum class OpCode : unsigned char
    {
        Continuation = 0x0,
        Text = 0x1,
        Binary = 0x2,
        // 0x3-0x7 reserved for further non-control frames
        ConnectionClose = 0x8,
        Ping = 0x9,
        Pong = 0xA
        // 0xB-0xF are reserved for further control frames
    };

public:
    WebSocketFrame();
    WebSocketFrame(const Header & header);
    WebSocketFrame(OpCode opCode, bool isFinal = true);
    WebSocketFrame(OpCode opCode, const QByteArray & content);

    void setHeader(const Header & header);
    const Header & getHeader() const;

    void setContent(const QByteArray & content);
    const QByteArray & getContent() const;

    void append(const QByteArray & data);

    void setMask(const std::array<char, 4> & mask);
    void setMask(int mask);
    void setRandomMask();

    OpCode getOpCode() const;

    bool isControlFrame() const;
    bool isFinal() const;
    bool isContinuationFrame() const;

    void writeTo(QIODevice & device) const;

protected:
    Header m_header;
    bool m_masked;
    std::array<char, 4> m_mask;
    QByteArray m_content;

    QByteArray toByteArray() const;
};

} // namespace tastefulserver
