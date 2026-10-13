#include "telegramBuilder.h"

QString zniChannelName(const ZniChannel channel)
{
    switch (channel) {
    case ZniChannel::Modem1:
        return QStringLiteral("Modem 1");
    case ZniChannel::Modem2:
        return QStringLiteral("Modem 2");
    case ZniChannel::Modem3:
        return QStringLiteral("Modem 3");
    case ZniChannel::Modem4:
        return QStringLiteral("Modem 4");
    case ZniChannel::Tty1:
        return QStringLiteral("TTY 1");
    case ZniChannel::Tty2:
        return QStringLiteral("TTY 2");
    }
    return QString();
}

Telegram TelegramBuilder::create(const ZniCommand command)
{
    Telegram telegram;
    telegram.setByte(0, static_cast<quint8>(command));
    // TODO: Bedeutung des zweiten Bytes (in der Testspezifikation immer 01) mit der Telegrammbeschreibung abgleichen
    telegram.setByte(1, 0x01);
    return telegram;
}

Telegram TelegramBuilder::openChannel(const ZniChannel channel)
{
    Telegram telegram = create(ZniCommand::OpenChannel);
    telegram.setByte(2, static_cast<quint8>(channel));
    return telegram;
}

Telegram TelegramBuilder::closeChannel(const ZniChannel channel)
{
    Telegram telegram = create(ZniCommand::CloseChannel);
    telegram.setByte(2, static_cast<quint8>(channel));
    return telegram;
}

Telegram TelegramBuilder::configure(const ZniChannel channel)
{
    Telegram telegram = create(ZniCommand::Configure);
    telegram.setByte(2, static_cast<quint8>(channel));
    return telegram;
}

Telegram TelegramBuilder::sendZlv(const ZniChannel modem, const QByteArray &info)
{
    Telegram telegram = create(ZniCommand::SendZlv);
    telegram.setByte(2, static_cast<quint8>(modem));
    for (int i = 0; i < info.size() && i < ZlvInfoSize; ++i) {
        telegram.setByte(4 + i, static_cast<quint8>(info.at(i)));
    }
    return telegram;
}

Telegram TelegramBuilder::sendTty(const ZniChannel tty, const QByteArray &payload)
{
    const int length = qMin(payload.size(), TtyMaxPayloadSize);

    Telegram telegram = create(ZniCommand::SendTty);
    telegram.setByte(2, static_cast<quint8>(tty));
    telegram.setByte(3, static_cast<quint8>(length));
    for (int i = 0; i < length; ++i) {
        telegram.setByte(4 + i, static_cast<quint8>(payload.at(i)));
    }
    return telegram;
}

Telegram TelegramBuilder::readSwitchCards(const quint16 cardMask)
{
    Telegram telegram = create(ZniCommand::ReadSwitchCards);
    telegram.setByte(2, static_cast<quint8>(cardMask >> 8));
    telegram.setByte(3, static_cast<quint8>(cardMask & 0xFF));
    // Byte 5 und 6 wie in der Testspezifikation (00 01)
    telegram.setByte(5, 0x01);
    return telegram;
}

Telegram TelegramBuilder::setSwitchCards(const quint16 cardMask, const QVector<quint16> &states)
{
    Telegram telegram = create(ZniCommand::SetSwitchCards);
    telegram.setByte(2, static_cast<quint8>(cardMask >> 8));
    telegram.setByte(3, static_cast<quint8>(cardMask & 0xFF));
    for (int i = 0; i < states.size() && i < SwitchCardCount; ++i) {
        telegram.setByte(4 + 2 * i, static_cast<quint8>(states.at(i) >> 8));
        telegram.setByte(5 + 2 * i, static_cast<quint8>(states.at(i) & 0xFF));
    }
    return telegram;
}

Telegram TelegramBuilder::reset()
{
    return create(ZniCommand::Reset);
}

Telegram TelegramBuilder::status(const quint8 page)
{
    Telegram telegram = create(ZniCommand::Status);
    telegram.setByte(2, page);
    return telegram;
}
