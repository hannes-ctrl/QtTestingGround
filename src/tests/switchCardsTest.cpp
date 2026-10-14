#include "switchCardsTest.h"

#include <QStringList>

#include "core/testContext.h"
#include "hardware/telegramBuilder.h"

SwitchCardsTest::SwitchCardsTest()
    : TestCase(QStringLiteral("Anschaltkarten-Tests"))
{
}

SwitchCardsTest::~SwitchCardsTest() = default;

TestResult SwitchCardsTest::run(TestContext &context)
{
    TestResult result(name());

    // Testspezifikation 5.1: Antwort meldet die Maske in Byte 3 und 4 zurück, danach je Karte 2 Byte Zustände.
    // TODO: Laut Testspezifikation reichen die Zustände nur bis Byte 24 (10 Karten); für 11 Karten
    //       wären es Byte 5 bis 26. Hier werden alle 11 gelesen, bis das geklärt ist.
    const quint16 mask = TelegramBuilder::AllSwitchCards;
    const ResponseCheck readStates = [mask](const Telegram &response, QString &message) {
        const quint16 echoedMask = static_cast<quint16>(response.number(2, 2));
        if (echoedMask != mask) {
            message = QStringLiteral("Antwort meldet Maske %1 statt %2")
                          .arg(QString::number(echoedMask, 16).rightJustified(4, QLatin1Char('0')).toUpper(),
                               QString::number(mask, 16).rightJustified(4, QLatin1Char('0')).toUpper());
            return false;
        }

        QStringList states;
        for (int card = 0; card < TelegramBuilder::SwitchCardCount; ++card) {
            states.append(QStringLiteral("X1%1: %2")
                              .arg(card + 1, 2, 10, QLatin1Char('0'))
                              .arg(response.number(4 + 2 * card, 2), 4, 16, QLatin1Char('0')));
        }
        message = QStringLiteral("Zustände: %1").arg(states.join(QStringLiteral(", ")).toUpper());
        return true;
    };
    result.addStep(sendStep(context, QStringLiteral("Zustand der Anschaltkarten 3314/3581 lesen"),
                            TelegramBuilder::readSwitchCards(mask), readStates));

    if (context.abortRequested()) {
        return result;
    }

    // Testspezifikation 5.2: erst alle Ausgänge ein, dann wieder aus, damit die Karten im Grundzustand enden
    const QVector<quint16> allOn(TelegramBuilder::SwitchCardCount, 0xFFFF);
    const QVector<quint16> allOff(TelegramBuilder::SwitchCardCount, 0x0000);
    result.addStep(sendStep(context, QStringLiteral("Anschaltkarten 3487 setzen: alle Ausgänge ein"),
                            TelegramBuilder::setSwitchCards(mask, allOn)));
    result.addStep(sendStep(context, QStringLiteral("Anschaltkarten 3487 setzen: alle Ausgänge aus"),
                            TelegramBuilder::setSwitchCards(mask, allOff)));
    return result;
}
