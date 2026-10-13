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
                               QStringLiteral("In der Testspezifikation offen (4.5)")));
    return result;
}

void TtyTest::testTty(TestContext &context, const ZniChannel tty, TestResult &result) const
{
    const QString channel = zniChannelName(tty);

    // TODO: Nutzdaten festlegen (Testspezifikation 4.4), bis dahin ein fester Prüftext
    const QByteArray payload = QByteArrayLiteral("ZNI-BG TTY-Test");

    result.addStep(sendStep(context, QStringLiteral("%1: konfigurieren").arg(channel),
                            TelegramBuilder::configure(tty)));
    result.addStep(sendStep(context, QStringLiteral("%1: Kanal öffnen").arg(channel),
                            TelegramBuilder::openChannel(tty)));
    result.addStep(sendStep(context, QStringLiteral("%1: Telegramm senden").arg(channel),
                            TelegramBuilder::sendTty(tty, payload)));
    result.addStep(sendStep(context, QStringLiteral("%1: Kanal schließen").arg(channel),
                            TelegramBuilder::closeChannel(tty)));
}
