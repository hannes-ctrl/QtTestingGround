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
    TtyReceived = 0x45,
    ReadSwitchCards = 0x4C,
    Configure = 0x4D,
    OpenChannel = 0x4F,
    Reset = 0x52,
    SetSwitchCards = 0x53,
    SendTty = 0x54,
    SendZlv = 0x5A,
    ZlvStatus = 0x62,
    Status = 0x73,
    ZlvReceived = 0x7A
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
 * @brief Konfiguration eines Kanals im transparenten Betrieb (Testspezifikation 3.1.1).
 */
struct TransparentConfig {
    /// Baudrate = Wert * 600 (01 bis 40), ab 41: (Wert - 40) * 57600
    quint8 baudRate = 0x01;
    /// 05 bis 08; Modem 1 bis 3 sind auf 8 Datenbits festgelegt
    quint8 dataBits = 0x08;
    /// 00 keine, 01 gerade, 02 ungerade
    quint8 parity = 0x00;
    /// 01 oder 02; Modem 1 bis 3 sind auf 1 Stoppbit festgelegt
    quint8 stopBits = 0x01;
};

/**
 * @brief Konfiguration eines Modems als ZLV-Bus-Teilnehmer (Testspezifikation 3.1.2).
 *
 * TODO: Die in der Testspezifikation offenen Bytes (eigene Unterstationsnummer, Anzahl der
 *       Unterstationen, CTS-Verzögerung, Sendepause, RTS-Abfallverzögerung, Sendeverzögerung
 *       nach CTS) stehen auf 00, bis ihre Werte feststehen.
 */
struct ZlvConfig {
    quint8 ownStationNumber = 0x00;
    quint8 stationCount = 0x00;
    /// 00 kein Löschen, 01 bis FE Sekunden nach ZLV-Telegramm, FF Standard (20 s)
    quint8 clearSendBuffer = 0xFF;
    quint8 ctsDelay = 0x00;
    quint8 sendPause = 0x00;
    /// zyklische Statusmeldungen (Telegramm 62)
    bool cyclicStatus = false;
    quint8 minRtsDelay = 0x00;
    quint8 sendDelayAfterCts = 0x00;
    /// 00 keine Reaktivierung, 01 bis FF Sekunden bis zum Reaktivierungstelegramm
    quint8 reactivation = 0x00;
    /// 00 vollwertiger Teilnehmer, 01 vollwertig nach wiederholender US-Nummer, 02 nur lesend
    quint8 mode = 0x00;
};

/**
 * @brief Baut die Telegramme der Testspezifikation. Hier steht der Aufbau der Telegramme an einer Stelle,
 *        die Tests arbeiten nur mit sprechenden Methoden.
 *
 * Byte-Angaben in den Kommentaren zählen wie in der Testspezifikation ab 1.
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

    /// Statusseiten für status()
    static constexpr quint8 StatusPageGeneral = 0x00;
    static constexpr quint8 StatusPageSerialNumber = 0x01;
    static constexpr quint8 StatusPageVersions = 0x02;
    static constexpr quint8 StatusPageModemOverflow = 0x03;
    static constexpr quint8 StatusPageTtyCanOverflow = 0x04;

    static Telegram configureTransparent(ZniChannel channel, const TransparentConfig &config);
    static Telegram configureZlv(ZniChannel modem, const ZlvConfig &config);

    /**
     * @param closeAfterTcpLoss 00: Kanal nur durch Kommando 43 schließen, 01 bis FF: nach so vielen
     *        Sekunden ohne TCP-Verbindung
     */
    static Telegram openChannel(ZniChannel channel, quint8 closeAfterTcpLoss = 0x00);
    static Telegram closeChannel(ZniChannel channel);

    /**
     * @brief ZLV-Telegramm über ein Modem senden.
     * @param info ZLV-Informationstelegramm, höchstens 12 Byte; kürzere Daten werden mit 00 aufgefüllt
     */
    static Telegram sendZlv(ZniChannel modem, const QByteArray &info, bool repeat = false);

    /**
     * @brief Telegramm über eine TTY-Schnittstelle senden.
     * @param payload Nutzdaten, höchstens 36 Byte; die Länge wird ins vierte Byte eingetragen
     */
    static Telegram sendTty(ZniChannel tty, const QByteArray &payload);

    /**
     * @brief Zustand der Anschaltkarten 3314/3581 anfordern.
     * @param cardMask ein Bit je Karte, Bit 0 = X101 bis Bit 10 = X111
     * @param cyclic Zustände zyklisch senden
     * @param cycleTime Zykluszeit = Wert * 100 ms (01 bis FF)
     * @param resetBeforeEachScan 00: Reset nach letzter Abtastung, 01: vor jeder Abtastung
     */
    static Telegram readSwitchCards(quint16 cardMask, bool cyclic = false, quint8 cycleTime = 0x01,
                                    bool resetBeforeEachScan = false);

    /**
     * @brief Ausgänge der Anschaltkarten 3487 setzen.
     * @param cardMask ein Bit je Karte, Bit 0 = X101 bis Bit 10 = X111
     * @param states Zustände je Steckplatz X101 bis X111 (ein Bit je Ausgang), höchstens 11 Einträge
     *
     * TODO: Annahme, dass jeder Zustand mit dem höherwertigen Byte zuerst übertragen wird.
     */
    static Telegram setSwitchCards(quint16 cardMask, const QVector<quint16> &states);

    static Telegram reset();

    /**
     * @param page angeforderte Seite, siehe StatusPage...
     * @param tcpMonitoring TCP-Verbindungsüberwachungszeit in Sekunden, 00 = keine
     */
    static Telegram status(quint8 page, quint8 tcpMonitoring = 0x00);

private:
    /**
     * @brief Telegramm mit Kommando im ersten und 01 im zweiten Byte, Rest 00.
     */
    static Telegram create(ZniCommand command);
};

#endif //ZNI_BG_TESTING_TELEGRAMBUILDER_H
