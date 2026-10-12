#include "telegram.h"

Telegram::Telegram()
    : m_data(Size, '\0')
{
}

Telegram::Telegram(const QByteArray &data)
    : m_data(data)
{
    if (m_data.size() < Size) {
        m_data.append(QByteArray(Size - m_data.size(), '\0'));
    }
}

Telegram::~Telegram() = default;

Telegram Telegram::fromHex(const QString &hex)
{
    return Telegram(QByteArray::fromHex(hex.toLatin1()));
}

bool Telegram::isValid() const
{
    return m_data.size() == Size;
}

quint8 Telegram::command() const
{
    return byteAt(0);
}

quint8 Telegram::byteAt(const int index) const
{
    if (index < 0 || index >= m_data.size()) {
        return 0;
    }
    return static_cast<quint8>(m_data.at(index));
}

void Telegram::setByte(const int index, const quint8 value)
{
    if (index < 0 || index >= m_data.size()) {
        return;
    }
    m_data[index] = static_cast<char>(value);
}

QByteArray Telegram::data() const
{
    return m_data;
}

QString Telegram::toHex() const
{
    return QString::fromLatin1(m_data.toHex(' ').toUpper());
}

bool Telegram::operator==(const Telegram &other) const
{
    return m_data == other.m_data;
}

bool Telegram::operator!=(const Telegram &other) const
{
    return !(*this == other);
}
