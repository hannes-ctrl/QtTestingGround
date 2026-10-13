#ifndef ZNI_BG_TESTING_TELEGRAMBUILDER_H
#define ZNI_BG_TESTING_TELEGRAMBUILDER_H

#include <QByteArray>
#include <QString>
#include <QVector>

#include "telegram.h"

/**
 * @brief Kommandos (erstes Byte eines Telegramms) laut Testspezifikation.
 */
enum class ZniCommand : quint8 {
    CloseChannel = 0x43,
    ReadSwitchCards = 0x4C,
    Configure = 0x4D,
    OpenChannel = 0x4F,
    Reset = 0x52,
    SetSwitchCards = 0x53,
    SendTty = 0x54,
    SendZlv = 0x5A,
    Status = 0x73
};

/**
 * @brief Kanäle (drittes Byte) der Modems am ZLV-Bus und der TTY-Schnittstellen.
 */
enum class ZniChannel : quint8 {
    Modem1 = 0x79,
    Modem2 = 0x7A,
    Modem3 = 0x7B,
    Modem4 = 0x7C,
    Tty1 = 0x7D,
    Tty2 = 0x7E
};

/**
 * @brief Anzeigename eines Kanals, z. B. "Modem 1".
 */
QString zniChannelName(ZniChannel channel);

/**
 * @brief Baut die Telegramme der Testspezifikation. Hier steht der Aufbau der Telegramme an einer Stelle,
 *        die Tests arbeiten nur mit sprechenden Methoden.
 */
class TelegramBuilder {
public:
    /// Nutzdaten eines ZLV-Informationstelegramms (Byte 5 bis 16)
    static constexpr int ZlvInfoSize = 12;
    /// Maximale Nutzdaten eines TTY-Telegramms (Byte 5 bis 40)
    static constexpr int TtyMaxPayloadSize = 36;
    /// Anzahl der Anschaltkarten-Steckplätze (X101 bis X111)
    static constexpr int SwitchCardCount = 11;
    /// Maske, die alle Anschaltkarten auswählt
    static constexpr quint16 AllSwitchCards = 0x07FF;

    /// Statusseite mit dem allgemeinen Status
    static constexpr quint8 StatusPageGeneral = 0x00;
    /// Statusseite mit der Seriennummer
    static constexpr quint8 StatusPageSerialNumber = 0x01;

    static Telegram openChannel(ZniChannel channel);
    static Telegram closeChannel(ZniChannel channel);

    /**
     * @brief Konfiguriert ein Modem oder eine TTY-Schnittstelle.
     *
     * TODO: Die Konfigurationsbytes (Baudrate, Datenlänge, Parität, Stoppbit) sind in der
     *       Testspezifikation noch nicht festgelegt und bleiben 00.
     */
    static Telegram configure(ZniChannel channel);

    /**
     * @brief ZLV-Telegramm über ein Modem senden.
     * @param info ZLV-Informationstelegramm, höchstens 12 Byte; kürzere Daten werden mit 00 aufgefüllt
     */
    static Telegram sendZlv(ZniChannel modem, const QByteArray &info);

    /**
     * @brief Telegramm über eine TTY-Schnittstelle senden.
     * @param payload Nutzdaten, höchstens 36 Byte; die Länge wird ins vierte Byte eingetragen
     */
    static Telegram sendTty(ZniChannel tty, const QByteArray &payload);

    /**
     * @brief Zustand der Anschaltkarten 3314 anfordern.
     * @param cardMask ein Bit je Karte, Bit 0 = X101 bis Bit 10 = X111
     */
    static Telegram readSwitchCards(quint16 cardMask);

    /**
     * @brief Ausgänge der Anschaltkarten 3487 setzen.
     * @param cardMask ein Bit je Karte, Bit 0 = X101 bis Bit 10 = X111
     * @param states Zustände je Steckplatz (ein Bit je Ausgang), höchstens 11 Einträge
     *
     * TODO: Annahme, dass die Zustände in der Reihenfolge X101 bis X111 mit dem höherwertigen
     *       Byte zuerst übertragen werden; mit der Telegrammbeschreibung abgleichen.
     */
    static Telegram setSwitchCards(quint16 cardMask, const QVector<quint16> &states);

    static Telegram reset();

    /**
     * @brief Status des ZNI anfordern.
     * @param page angeforderte Seite, siehe StatusPageGeneral und StatusPageSerialNumber
     */
    static Telegram status(quint8 page);

private:
    /**
     * @brief Telegramm mit Kommando im ersten und 01 im zweiten Byte, Rest 00.
     */
    static Telegram create(ZniCommand command);
};

#endif //ZNI_BG_TESTING_TELEGRAMBUILDER_H
