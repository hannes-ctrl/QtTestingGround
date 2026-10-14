#include "zniErrorCode.h"

#include <QHash>

namespace {

const QHash<quint8, QString> &descriptions()
{
    static const QHash<quint8, QString> table = {
        {0x00, QStringLiteral("OK")},
        {0x01, QStringLiteral("Zustände der Anschaltkarten anfordern: Keine Anschaltkarte ausgewählt")},
        {0x02, QStringLiteral("Zustände der Anschaltkarten anfordern: Ungültige Zykluszeit")},
        {0x03, QStringLiteral("Zustände der Anschaltkarten anfordern: Maskenkonflikt zwischen anfordern und setzen (Steckplatz doppelt ausgewählt)")},
        {0x08, QStringLiteral("Anschaltkarten setzen: Keine Karten ausgewählt")},
        {0x09, QStringLiteral("Anschaltkarten setzen: Maskenkonflikt zwischen anfordern und setzen")},
        {0x20, QStringLiteral("Modem und TTY konfigurieren: Ungültiges Byte im Kommandotelegramm")},
        {0x21, QStringLiteral("Modem und TTY konfigurieren: Kanal ist nicht geschlossen")},
        {0x22, QStringLiteral("Modem und TTY konfigurieren: Ungültiger Kanal")},
        {0x23, QStringLiteral("Modem und TTY konfigurieren: Ungültige Baudrate")},
        {0x24, QStringLiteral("Modem und TTY konfigurieren: Ungültige Konfiguration (Datenlänge, Parität, Stoppbit)")},
        {0x25, QStringLiteral("Modem und TTY konfigurieren: Serial Clock nicht verfügbar (nur bei Modem 4 verfügbar)")},
        {0x30, QStringLiteral("CAN konfigurieren: Kanal ist nicht geschlossen")},
        {0x31, QStringLiteral("CAN konfigurieren: Ungültiger Kanal")},
        {0x32, QStringLiteral("CAN konfigurieren: Ungültige Datenrate")},
        {0x40, QStringLiteral("Kanal öffnen: Kanal noch nicht konfiguriert oder bereits geöffnet")},
        {0x41, QStringLiteral("Kanal öffnen: ungültiger Kanal")},
        {0x50, QStringLiteral("Kanal schließen: Kanal ist bereits geschlossen")},
        {0x51, QStringLiteral("Kanal schließen: ungültiger Kanal")},
        {0x60, QStringLiteral("ZLV-Telegramm senden: Kanal ist nicht offen oder nicht konfiguriert")},
        {0x61, QStringLiteral("ZLV-Telegramm senden: Pufferüberlauf")},
        {0x62, QStringLiteral("ZLV-Telegramm senden: Telegramm während ZLV-Bus-Störung erhalten")},
        {0x63, QStringLiteral("ZLV-Telegramm senden: ungültiger Kanal")},
        {0x64, QStringLiteral("ZLV-Telegramm senden: ungültige Betriebsart zum Senden")},
        {0x70, QStringLiteral("ZLV-Telegramm empfangen: Das empfangene Telegramm enthält einen Paritätsfehler (Byte- oder Längsparität)")},
        {0x71, QStringLiteral("ZLV-Telegramm empfangen: Das empfangene Telegramm enthält mehr Bytes als durch die Telegrammart vorgegeben ist")},
        {0x72, QStringLiteral("ZLV-Telegramm empfangen: Das empfangene Telegramm enthält weniger Bytes als durch die Telegrammart vorgegeben ist")},
        {0x90, QStringLiteral("CAN-Telegramm senden: Kanal ist nicht offen oder nicht konfiguriert")},
        {0x91, QStringLiteral("CAN-Telegramm senden: Pufferüberlauf")},
        {0x92, QStringLiteral("CAN-Telegramm senden: ungültiger Kanal")},
        {0x93, QStringLiteral("CAN-Telegramm senden: ungültige CAN-ID oder Datenlänge > 8")},
        {0xB0, QStringLiteral("Telegramm senden: Kanal ist nicht offen oder nicht konfiguriert")},
        {0xB1, QStringLiteral("Telegramm senden: Pufferüberlauf")},
        {0xB2, QStringLiteral("Telegramm senden: maximale Datenlänge überschritten (ungültige Eingabe)")},
        {0xB3, QStringLiteral("Telegramm senden: ungültiger Kanal")},
        {0xE0, QStringLiteral("Status übermitteln: ungültige Seite angegeben")},
        {0xF8, QStringLiteral("GPIO setzen: Ungültige Zeitangabe für Rechteck")},
    };
    return table;
}

}

bool ZniErrorCode::isKnown(const quint8 code)
{
    return descriptions().contains(code);
}

QString ZniErrorCode::description(const quint8 code)
{
    const auto it = descriptions().constFind(code);
    if (it == descriptions().constEnd()) {
        const QString hex = QStringLiteral("%1").arg(code, 2, 16, QLatin1Char('0')).toUpper();
        return QStringLiteral("Unbekannter Fehlercode %1").arg(hex);
    }
    return it.value();
}
