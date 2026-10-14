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
    // TODO: Bedeutung des zweiten Bytes (in der Testspezifikation immer 01) klären
    telegram.setByte(1, 0x01);
    return telegram;
}

Telegram TelegramBuilder::configureTransparent(const ZniChannel channel, const TransparentConfig &config)
{
    Telegram telegram = create(ZniCommand::Configure);
    telegram.setByte(2, static_cast<quint8>(channel));
    telegram.setByte(3, 0x00);              // Byte 4: transparenter Betrieb
    telegram.setByte(4, config.baudRate);
    telegram.setByte(5, config.dataBits);
    telegram.setByte(6, config.parity);
    telegram.setByte(7, config.stopBits);
    telegram.setByte(8, 0xFF);              // Byte 9 und 10 wie in der Testspezifikation (FF 00)
    return telegram;
}

Telegram TelegramBuilder::configureZlv(const ZniChannel modem, const ZlvConfig &config)
{
    Telegram telegram = create(ZniCommand::Configure);
    telegram.setByte(2, static_cast<quint8>(modem));
    telegram.setByte(3, 0xFF);              // Byte 4: ZLV-Bus-Teilnehmer
    telegram.setByte(8, 0xFF);              // Byte 9 wie in der Testspezifikation
    telegram.setByte(9, config.ownStationNumber);
    telegram.setByte(10, config.stationCount);
    telegram.setByte(11, config.clearSendBuffer);
    telegram.setByte(12, config.ctsDelay);
    telegram.setByte(13, config.sendPause);
    telegram.setByte(14, config.cyclicStatus ? 0xFF : 0x00);
    telegram.setByte(15, config.minRtsDelay);
    telegram.setByte(16, config.sendDelayAfterCts);
    telegram.setByte(17, config.reactivation);
    telegram.setByte(18, config.mode);
    return telegram;
}

Telegram TelegramBuilder::openChannel(const ZniChannel channel, const quint8 closeAfterTcpLoss)
{
    Telegram telegram = create(ZniCommand::OpenChannel);
    telegram.setByte(2, static_cast<quint8>(channel));
    telegram.setByte(3, closeAfterTcpLoss);
    return telegram;
}

Telegram TelegramBuilder::closeChannel(const ZniChannel channel)
{
    Telegram telegram = create(ZniCommand::CloseChannel);
    telegram.setByte(2, static_cast<quint8>(channel));
    return telegram;
}

Telegram TelegramBuilder::sendZlv(const ZniChannel modem, const QByteArray &info, const bool repeat)
{
    Telegram telegram = create(ZniCommand::SendZlv);
    telegram.setByte(2, static_cast<quint8>(modem));
    telegram.setByte(3, repeat ? 0xFF : 0x00);
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

Telegram TelegramBuilder::readSwitchCards(const quint16 cardMask, const bool cyclic, const quint8 cycleTime,
                                          const bool resetBeforeEachScan)
{
    Telegram telegram = create(ZniCommand::ReadSwitchCards);
    telegram.setByte(2, static_cast<quint8>(cardMask >> 8));
    telegram.setByte(3, static_cast<quint8>(cardMask & 0xFF));
    telegram.setByte(4, cyclic ? 0x01 : 0x00);
    telegram.setByte(5, cycleTime);
    telegram.setByte(6, resetBeforeEachScan ? 0x01 : 0x00);
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

Telegram TelegramBuilder::status(const quint8 page, const quint8 tcpMonitoring)
{
    Telegram telegram = create(ZniCommand::Status);
    telegram.setByte(2, page);
    telegram.setByte(3, tcpMonitoring);
    return telegram;
}
