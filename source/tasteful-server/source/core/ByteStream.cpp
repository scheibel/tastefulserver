#include <tasteful-server/ByteStream.h>

namespace tastefulserver {

ByteStream::ByteStream(const QString & linebreak)
: m_linebreak(linebreak)
, m_pos(0)
{
}

ByteStream::ByteStream(const QByteArray & bytes, const QString & linebreak)
: m_buffer(bytes)
, m_linebreak(linebreak)
, m_pos(0)
{
}

ByteStream ByteStream::forLinebreak(const QString & linebreak)
{
    return ByteStream(QByteArray(), linebreak);
}

void ByteStream::append(const QByteArray & bytes)
{
    m_buffer.append(bytes);
}

void ByteStream::flush()
{
    m_buffer = m_buffer.remove(0, m_pos);
    m_pos = 0;
}

QByteArray ByteStream::alreadyRead() const
{
    return m_buffer.mid(0, m_pos);
}

bool ByteStream::canReadLine() const
{
    return m_buffer.indexOf(m_linebreak, m_pos)>=0;
}

int ByteStream::availableBytes() const
{
    return m_buffer.size() - m_pos;
}

char ByteStream::readByte()
{
    if (atEnd())
        return 0;

    char c = m_buffer[m_pos];
    ++m_pos;

    return c;
}

QByteArray ByteStream::read(int length)
{
    if (availableBytes()<length)
    {
        m_pos = m_buffer.size();

        return m_buffer.mid(m_pos);
    }
    QByteArray bytes = m_buffer.mid(m_pos, length);
    m_pos += length;

    return bytes;
}

void ByteStream::skip(int length)
{
    if (availableBytes()<length)
    {
        m_pos = m_buffer.size();

        return;
    }
    m_pos += length;
}

QString ByteStream::readLine()
{
    int p = m_buffer.indexOf(m_linebreak, m_pos);

    if (p<0)
    {
        return readAll();
    }
    QString line = QString(m_buffer.mid(m_pos, p - m_pos));
    m_pos = p + m_linebreak.size();

    return line;
}

bool ByteStream::canReadUpTo(const QString & delimiter) const
{
    return m_buffer.indexOf(delimiter, m_pos)>=0;
}

bool ByteStream::canReadUpTo(const QChar & delimiter) const
{
    return m_buffer.indexOf(delimiter, m_pos)>=0;
}

void ByteStream::skipUpTo(const QString & delimiter)
{
    int p = m_buffer.indexOf(delimiter, m_pos);

    if (p<0)
    {
        skipAll();
    }
    else
    {
        m_pos = p;
    }
}

void ByteStream::skipUpTo(const QChar & delimiter)
{
    int p = m_buffer.indexOf(delimiter, m_pos);

    if (p<0)
    {
        skipAll();
    }
    else
    {
        m_pos = p;
    }
}

void ByteStream::skipBehind(const QString & delimiter)
{
    skipUpTo(delimiter);
    skip(delimiter.size());
}

void ByteStream::skipBehind(const QChar & delimiter)
{
    skipUpTo(delimiter);
    skip(1);
}

QByteArray ByteStream::readUpTo(const QString & delimiter, bool skipDelimiter)
{
    int p = m_buffer.indexOf(delimiter, m_pos);

    if (p<0)
    {
        return readAll();
    }
    QByteArray part = m_buffer.mid(m_pos, p - m_pos);
    m_pos = p;
    if (skipDelimiter)
    {
        skip(delimiter.size());
    }

    return part;
}

QByteArray ByteStream::readUpTo(const QChar & delimiter, bool skipDelimiter)
{
    int p = m_buffer.indexOf(delimiter, m_pos);

    if (p<0)
    {
        return readAll();
    }
    QByteArray part = m_buffer.mid(m_pos, p - m_pos);
    m_pos = p;
    if (skipDelimiter)
    {
        skip(1);
    }

    return part;
}

void ByteStream::skipAll()
{
    m_pos = m_buffer.size();
}

QByteArray ByteStream::readAll()
{
    QByteArray bytes = m_buffer.mid(m_pos);

    m_pos = m_buffer.size();

    return bytes;
}

bool ByteStream::atEnd() const
{
    return m_pos == m_buffer.size();
}

} // namespace tastefulserver
