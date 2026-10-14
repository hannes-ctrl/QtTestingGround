#include "zlvTest.h"

#include <QDeadlineTimer>
#include <QLocale>

#include "config/config.h"
#include "core/testContext.h"
#include "hardware/zniConnection.h"
#include "hardware/zniErrorCode.h"

namespace {

QString busStatusText(const quint8 status)
{
    switch (status) {
    case 0x00:
        return QStringLiteral("Einschaltroutine");
    case 0x01:
        return QStringLiteral("regulärer Betrieb");
    case 0x02:
        return QStringLiteral("Reaktivierung");
    case 0x03:
        return QStringLiteral("keine Verbindung zum Modem");
    }
    return QStringLiteral("unbekannt");
}

}

ZlvTest::ZlvTest()
    : TestCase(QStringLiteral("ZLV-Bus-Tests"))
{
}

ZlvTest::~ZlvTest() = default;

TestResult ZlvTest::run(TestContext &context)
{
    TestResult result(name());

    const ZniChannel modems[] = {ZniChannel::Modem1, ZniChannel::Modem2, ZniChannel::Modem3, ZniChannel::Modem4};
    for (const ZniChannel modem : modems) {
        if (context.abortRequested()) {
            return result;
        }
        testModem(context, modem, result);
    }

    result.addStep(skippedStep(QStringLiteral("ZLV-Telegramm empfangen"),
                               QStringLiteral("Gegenstelle, die ein ZLV-Telegramm sendet, ist nicht festgelegt (3.5)")));
    return result;
}

void ZlvTest::testModem(TestContext &context, const ZniChannel modem, TestResult &result) const
{
    const QString channel = zniChannelName(modem);
    const quint8 channelByte = static_cast<quint8>(modem);

    result.addStep(sendStep(context, QStringLiteral("%1: konfigurieren (transparent)").arg(channel),
                            TelegramBuilder::configureTransparent(modem, TransparentConfig()),
                            echoesThirdByte(channelByte)));

    // Zyklische Statusmeldungen einschalten, damit der ZLV-Status geprüft werden kann
    ZlvConfig zlvConfig;
    zlvConfig.cyclicStatus = true;
    result.addStep(sendStep(context, QStringLiteral("%1: konfigurieren (ZLV-Bus-Teilnehmer)").arg(channel),
                            TelegramBuilder::configureZlv(modem, zlvConfig), echoesThirdByte(channelByte)));

    result.addStep(sendStep(context, QStringLiteral("%1: Kanal öffnen").arg(channel),
                            TelegramBuilder::openChannel(modem), echoesThirdByte(channelByte)));

    result.addStep(receiveZlvStatus(context, modem));

    // TODO: Inhalt des ZLV-Informationstelegramms festlegen (Testspezifikation 3.4), bis dahin 12 Nullbytes
    const QByteArray info(TelegramBuilder::ZlvInfoSize, '\0');
    const Telegram request = TelegramBuilder::sendZlv(modem, info);
    const ResponseCheck echoesInfo = [channelByte, request](const Telegram &response, QString &message) {
        if (!echoesThirdByte(channelByte)(response, message)) {
            return false;
        }
        // Anfrage: Informationstelegramm in Byte 5 bis 16, Antwort: in Byte 4 bis 15 (Testspezifikation 3.4)
        const QByteArray sent = request.bytes(4, TelegramBuilder::ZlvInfoSize);
        const QByteArray echoed = response.bytes(3, TelegramBuilder::ZlvInfoSize);
        if (echoed != sent) {
            message = QStringLiteral("Zurückgemeldetes Informationstelegramm %1 weicht vom gesendeten %2 ab")
                          .arg(QString::fromLatin1(echoed.toHex(' ').toUpper()),
                               QString::fromLatin1(sent.toHex(' ').toUpper()));
            return false;
        }
        return true;
    };
    result.addStep(sendStep(context, QStringLiteral("%1: ZLV-Telegramm senden").arg(channel), request, echoesInfo));

    result.addStep(sendStep(context, QStringLiteral("%1: Kanal schließen").arg(channel),
                            TelegramBuilder::closeChannel(modem), echoesThirdByte(channelByte)));
}

StepResult ZlvTest::receiveZlvStatus(TestContext &context, const ZniChannel modem) const
{
    const QString stepName = QStringLiteral("%1: ZLV-Status empfangen").arg(zniChannelName(modem));
    const int timeoutMs = context.config()->receiveTimeoutMs();
    const QDeadlineTimer deadline(timeoutMs);

    // Meldungen anderer Modems überspringen, bis die des gesuchten eintrifft
    Telegram status;
    do {
        if (!context.connection()->receive(static_cast<quint8>(ZniCommand::ZlvStatus), status,
                                           static_cast<int>(qMax<qint64>(0, deadline.remainingTime())))) {
            return StepResult(stepName, TestStatus::Failed,
                              QStringLiteral("Keine Statusmeldung innerhalb von %1 ms (%2)")
                                  .arg(timeoutMs).arg(context.connection()->errorString()));
        }
    } while (status.byteAt(2) != static_cast<quint8>(modem));

    TestStatus result = TestStatus::Passed;
    QString message;
    const quint8 busStatus = status.byteAt(3);
    if (status.errorCode() != 0x00) {
        result = TestStatus::Failed;
        message = QStringLiteral("Fehlercode %1: %2")
                      .arg(hexByte(status.errorCode()), ZniErrorCode::description(status.errorCode()));
    } else if (busStatus == 0x03) {
        result = TestStatus::Failed;
        message = QStringLiteral("Bus-Status: %1").arg(busStatusText(busStatus));
    } else {
        // Busumlaufzeit in Byte 5: Wert * 0,1 s
        message = QStringLiteral("Bus-Status: %1, letzte Busumlaufzeit: %2 s, nicht auswertbare Telegramme: %3")
                      .arg(busStatusText(busStatus))
                      .arg(QLocale(QLocale::German).toString(status.byteAt(4) / 10.0, 'f', 1))
                      .arg(status.byteAt(5));
    }

    StepResult step(stepName, result, message);
    step.setReceived(status.toHex());
    return step;
}
