#include "ttyTest.h"

#include "core/testContext.h"

TtyTest::TtyTest()
    : TestCase(QStringLiteral("TTY-Schnittstellen-Tests"))
{
}

TtyTest::~TtyTest() = default;

TestResult TtyTest::run(TestContext &context)
{
    TestResult result(name());

    const ZniChannel ttys[] = {ZniChannel::Tty1, ZniChannel::Tty2};
    for (const ZniChannel tty : ttys) {
        if (context.abortRequested()) {
            return result;
        }
        testTty(context, tty, result);
    }

    result.addStep(skippedStep(QStringLiteral("Telegramm empfangen"),
                               QStringLiteral("Gegenstelle, die ein TTY-Telegramm sendet, ist nicht festgelegt (4.5)")));
    return result;
}

void TtyTest::testTty(TestContext &context, const ZniChannel tty, TestResult &result) const
{
    const QString channel = zniChannelName(tty);
    const quint8 channelByte = static_cast<quint8>(tty);

    // TODO: Testspezifikation 4.1 lässt offen, ob TTY transparent oder als ZLV-Bus-Teilnehmer läuft;
    //       bis dahin transparent mit den Standardwerten aus TransparentConfig
    result.addStep(sendStep(context, QStringLiteral("%1: konfigurieren (transparent)").arg(channel),
                            TelegramBuilder::configureTransparent(tty, TransparentConfig()),
                            echoesThirdByte(channelByte)));
    result.addStep(sendStep(context, QStringLiteral("%1: Kanal öffnen").arg(channel),
                            TelegramBuilder::openChannel(tty), echoesThirdByte(channelByte)));

    // TODO: Nutzdaten festlegen, bis dahin ein fester Prüftext
    const QByteArray payload = QByteArrayLiteral("ZNI-BG TTY-Test");
    result.addStep(sendStep(context, QStringLiteral("%1: Telegramm senden").arg(channel),
                            TelegramBuilder::sendTty(tty, payload), echoesThirdByte(channelByte)));

    result.addStep(sendStep(context, QStringLiteral("%1: Kanal schließen").arg(channel),
                            TelegramBuilder::closeChannel(tty), echoesThirdByte(channelByte)));
}
