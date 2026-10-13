#include "zlvTest.h"

#include "core/testContext.h"
#include "hardware/telegramBuilder.h"

ZlvTest::ZlvTest()
    : TestCase(QStringLiteral("ZLV-Bus-Tests"))
{
}

ZlvTest::~ZlvTest() = default;

TestResult ZlvTest::run(TestContext &context)
{
    TestResult result(name());

    result.addStep(skippedStep(QStringLiteral("Modem konfigurieren"),
                               QStringLiteral("In der Testspezifikation offen (3.1)")));

    const ZniChannel modems[] = {ZniChannel::Modem1, ZniChannel::Modem2, ZniChannel::Modem3, ZniChannel::Modem4};
    for (const ZniChannel modem : modems) {
        if (context.abortRequested()) {
            return result;
        }
        testModem(context, modem, result);
    }

    result.addStep(skippedStep(QStringLiteral("ZLV-Telegramm empfangen"),
                               QStringLiteral("In der Testspezifikation offen (3.5)")));
    result.addStep(skippedStep(QStringLiteral("ZLV-Status"),
                               QStringLiteral("In der Testspezifikation offen (3.6)")));
    return result;
}

void ZlvTest::testModem(TestContext &context, const ZniChannel modem, TestResult &result) const
{
    const QString channel = zniChannelName(modem);

    // TODO: Inhalt des ZLV-Informationstelegramms festlegen (Testspezifikation 3.4), bis dahin 12 Nullbytes
    const QByteArray info(TelegramBuilder::ZlvInfoSize, '\0');

    result.addStep(sendStep(context, QStringLiteral("%1: Kanal öffnen").arg(channel),
                            TelegramBuilder::openChannel(modem)));
    result.addStep(sendStep(context, QStringLiteral("%1: ZLV-Telegramm senden").arg(channel),
                            TelegramBuilder::sendZlv(modem, info)));
    result.addStep(sendStep(context, QStringLiteral("%1: Kanal schließen").arg(channel),
                            TelegramBuilder::closeChannel(modem)));
}
