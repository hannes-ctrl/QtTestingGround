#include "miscTest.h"

#include <QStringList>

#include "core/testContext.h"
#include "hardware/telegramBuilder.h"

namespace {

QString resetSourceText(const quint8 source)
{
    switch (source) {
    case 0x00:
        return QStringLiteral("First power-up reset");
    case 0x01:
        return QStringLiteral("Return from Backup Mode");
    case 0x02:
        return QStringLiteral("Watchdog fault occurred");
    case 0x03:
        return QStringLiteral("Processor reset required by software");
    case 0x04:
        return QStringLiteral("NRST pin detected low");
    }
    return QStringLiteral("unbekannt");
}

QString boardErrorText(const quint8 error)
{
    switch (error) {
    case 0x00:
        return QStringLiteral("kein Fehler");
    case 0x01:
        return QStringLiteral("SD-Karte konnte nicht initialisiert werden");
    case 0x02:
        return QStringLiteral("keine SD-Karte gefunden");
    case 0x03:
        return QStringLiteral("Fehler beim Mount der SD-Karte");
    case 0x04:
        return QStringLiteral("keine Konfiguration auf der SD-Karte");
    case 0x05:
        return QStringLiteral("ungültige MAC-Adresse");
    case 0x06:
        return QStringLiteral("Reserve");
    case 0x07:
        return QStringLiteral("Fehler in der Konfiguration auf der SD-Karte");
    case 0x08:
        return QStringLiteral("ungültige Hexschalterstellung");
    case 0x20:
        return QStringLiteral("Fehler Stack-Test");
    case 0x21:
        return QStringLiteral("Fehler RAM-Test");
    case 0x22:
        return QStringLiteral("Fehler ROM-Test");
    }
    return QStringLiteral("unbekannter Baugruppenfehler");
}

/**
 * @brief Überlaufzähler (je 4 Byte) als Text, z. B. "Modem 1: 0, Modem 2: 3".
 */
QString overflowCounters(const Telegram &response, const int firstIndex, const QStringList &names)
{
    QStringList counters;
    for (int i = 0; i < names.size(); ++i) {
        counters.append(QStringLiteral("%1: %2").arg(names.at(i)).arg(response.number(firstIndex + 4 * i, 4)));
    }
    return counters.join(QStringLiteral(", "));
}

}

MiscTest::MiscTest()
    : TestCase(QStringLiteral("Sonstige Tests"))
{
}

MiscTest::~MiscTest() = default;

TestResult MiscTest::run(TestContext &context)
{
    TestResult result(name());

    // Testspezifikation 7.2, Status 00: Baugruppenfehler in Byte 24 muss 00 sein
    const ResponseCheck checkGeneralStatus = [](const Telegram &response, QString &message) {
        const quint8 boardError = response.byteAt(23);
        message = QStringLiteral("Reset-Quelle: %1, Betriebsstunden: %2 s, Chiptemperatur: %3 °C, "
                                 "Hexschalter beim Start/aktuell: %4/%5, Baugruppe: %6")
                      .arg(resetSourceText(response.byteAt(2)))
                      .arg(response.number(3, 4))
                      .arg(response.number(7, 2))
                      .arg(response.byteAt(17))
                      .arg(response.byteAt(18))
                      .arg(boardErrorText(boardError));
        return boardError == 0x00;
    };
    result.addStep(sendStep(context, QStringLiteral("ZNI-Status abfragen"),
                            TelegramBuilder::status(TelegramBuilder::StatusPageGeneral), checkGeneralStatus));

    // Status 02: Hardware-Nummer in Byte 3 bis 12, Software-Nummer in Byte 13 bis 22
    const ResponseCheck readVersions = [](const Telegram &response, QString &message) {
        message = QStringLiteral("Hardware-Nummer: %1, Software-Nummer: %2")
                      .arg(response.ascii(2, 10), response.ascii(12, 10));
        return true;
    };
    result.addStep(sendStep(context, QStringLiteral("Versionsnummern abfragen"),
                            TelegramBuilder::status(TelegramBuilder::StatusPageVersions), readVersions));

    // Status 03 und 04: Überlaufzähler werden nur protokolliert
    const ResponseCheck readModemOverflow = [](const Telegram &response, QString &message) {
        message = overflowCounters(response, 2, {QStringLiteral("Modem 1"), QStringLiteral("Modem 2"),
                                                 QStringLiteral("Modem 3"), QStringLiteral("Modem 4")});
        return true;
    };
    result.addStep(sendStep(context, QStringLiteral("Überlaufzähler Modem abfragen"),
                            TelegramBuilder::status(TelegramBuilder::StatusPageModemOverflow), readModemOverflow));

    const ResponseCheck readTtyCanOverflow = [](const Telegram &response, QString &message) {
        message = overflowCounters(response, 2, {QStringLiteral("TTY 1"), QStringLiteral("TTY 2"),
                                                 QStringLiteral("CAN 1"), QStringLiteral("CAN 2")});
        return true;
    };
    result.addStep(sendStep(context, QStringLiteral("Überlaufzähler TTY und CAN abfragen"),
                            TelegramBuilder::status(TelegramBuilder::StatusPageTtyCanOverflow), readTtyCanOverflow));

    if (context.abortRequested()) {
        return result;
    }

    // Testspezifikation 7.1: Antwort enthält den Betriebsstundenzähler in Byte 3 bis 6
    const ResponseCheck readOperatingTime = [](const Telegram &response, QString &message) {
        message = QStringLiteral("Betriebsstunden: %1 s").arg(response.number(2, 4));
        return true;
    };
    result.addStep(sendStep(context, QStringLiteral("Reset"), TelegramBuilder::reset(), readOperatingTime));
    return result;
}
