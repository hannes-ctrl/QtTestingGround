#include "switchCardsTest.h"

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

    result.addStep(skippedStep(QStringLiteral("Zustand der Anschaltkarten 3314 lesen"),
                               QStringLiteral("Über das Sim-ZNI nicht auslesbar (5.1)")));
    result.addStep(skippedStep(QStringLiteral("Zustand der Sonderanschaltkarte 3581 lesen"),
                               QStringLiteral("In der Testspezifikation offen (5.2)")));

    if (context.abortRequested()) {
        return result;
    }

    // Alle Ausgänge aller Karten X101 bis X111 setzen (Testspezifikation 5.3)
    const QVector<quint16> allOutputsOn(TelegramBuilder::SwitchCardCount, 0xFFFF);
    result.addStep(sendStep(context, QStringLiteral("Anschaltkarten 3487 setzen: alle Ausgänge ein"),
                            TelegramBuilder::setSwitchCards(TelegramBuilder::AllSwitchCards, allOutputsOn)));
    return result;
}
