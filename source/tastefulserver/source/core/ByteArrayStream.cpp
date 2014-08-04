#include <tastefulserver/ByteArrayStream.h>

namespace tastefulserver {

ByteArrayStream::ByteArrayStream(const QByteArray & bytes, const QString & linebreak)
    : m_buffer(bytes)
    , m_linebreak(linebreak)
    , m_pos(0)
{
}

ByteArrayStream ByteArrayStream::forLinebreak(const QString & linebreak)
{
    return ByteArrayStream(QByteArray(), linebreak);
}

void ByteArrayStream::append(const QByteArray & bytes)
{
    m_buffer.append(bytes);
}

void ByteArrayStream::flush()
{
    m_buffer = m_buffer.remove(0, m_pos);
    m_pos = 0;
}

bool ByteArrayStream::canReadLine() const
{
    return m_buffer.indexOf(m_linebreak, m_pos)>=0;
}

int ByteArrayStream::availableBytes() const
{
    return m_buffer.size() - m_pos;
}

QByteArray ByteArrayStream::read(int length)
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

void ByteArrayStream::skip(int length)
{
    if (availableBytes()<length)
    {
        m_pos = m_buffer.size();

        return;
    }
    m_pos += length;
}

QString ByteArrayStream::readLine()
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

bool ByteArrayStream::canReadUpTo(const QString & delimiter) const
{
    return m_buffer.indexOf(delimiter, m_pos)>=0;
}

bool ByteArrayStream::canReadUpTo(const QChar & delimiter) const
{
    return m_buffer.indexOf(delimiter, m_pos)>=0;
}

void ByteArrayStream::skipUpTo(const QString & delimiter)
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

void ByteArrayStream::skipUpTo(const QChar & delimiter)
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

void ByteArrayStream::skipBehind(const QString & delimiter)
{
    skipUpTo(delimiter);
    skip(delimiter.size());
}

void ByteArrayStream::skipBehind(const QChar & delimiter)
{
    skipUpTo(delimiter);
    skip(1);
}

QByteArray ByteArrayStream::readUpTo(const QString & delimiter, bool skipDelimiter)
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

QByteArray ByteArrayStream::readUpTo(const QChar & delimiter, bool skipDelimiter)
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

void ByteArrayStream::skipAll()
{
    m_pos = m_buffer.size();
}

QByteArray ByteArrayStream::readAll()
{
    QByteArray bytes = m_buffer.mid(m_pos);

    m_pos = m_buffer.size();

    return bytes;
}

bool ByteArrayStream::atEnd() const
{
    return m_pos == m_buffer.size();
}

} // namespace tastefulserver
