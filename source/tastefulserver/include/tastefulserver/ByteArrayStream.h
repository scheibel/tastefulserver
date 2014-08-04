#pragma once

#include <QString>
#include <QByteArray>

#include <tastefulserver/tastefulserver_api.h>

namespace tastefulserver {

class TASTEFULSERVER_API ByteArrayStream
{
public:
    ByteArrayStream(const QByteArray & bytes, const QString & linebreak = "\n");

    static ByteArrayStream forLinebreak(const QString & linebreak);

    void append(const QByteArray & bytes);
    QByteArray read(int length);
    void skip(int length);
    QString readLine();
    void skipUpTo(const QString & delimiter);
    void skipUpTo(const QChar & delimiter);
    void skipBehind(const QString & delimiter);
    void skipBehind(const QChar & delimiter);
    bool canReadUpTo(const QString & delimiter) const;
    bool canReadUpTo(const QChar & delimiter) const;
    QByteArray readUpTo(const QString & delimiter, bool skipDelimiter = false);
    QByteArray readUpTo(const QChar & delimiter, bool skipDelimiter = false);
    QByteArray readAll();
    void skipAll();
    bool canReadLine() const;
    int availableBytes() const;
    void flush();
    bool atEnd() const;

protected:
    QByteArray m_buffer;
    QString m_linebreak;
    int m_pos;
};

} // namespace tastefulserver
