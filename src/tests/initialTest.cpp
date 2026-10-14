#include "initialTest.h"

#include "core/testContext.h"
#include "hardware/telegramBuilder.h"

InitialTest::InitialTest()
    : TestCase(QStringLiteral("Erste Tests"))
{
}

InitialTest::~InitialTest() = default;

TestResult InitialTest::run(TestContext &context)
{
    TestResult result(name());

    // Testspezifikation 2.1: Seriennummer steht in Byte 3 bis 31 als ASCII-Text
    const QString expected = context.serialNumber().trimmed();
    const ResponseCheck compareSerialNumber = [expected](const Telegram &response, QString &message) {
        const QString reported = response.ascii(2, 29).trimmed();
        if (reported != expected) {
            message = QStringLiteral("ZNI meldet \"%1\", eingegeben wurde \"%2\"").arg(reported, expected);
            return false;
        }
        message = QStringLiteral("Seriennummer %1").arg(reported);
        return true;
    };

    const QString stepName = QStringLiteral("Seriennummer prüfen");
    const Telegram request = TelegramBuilder::status(TelegramBuilder::StatusPageSerialNumber);
    if (expected.isEmpty()) {
        // Ohne Eingabe lässt sich nichts vergleichen; die gemeldete Nummer kommt trotzdem ins Protokoll
        Telegram response;
        StepResult step = sendStep(context, stepName, request, nullptr, &response);
        if (step.status() == TestStatus::Passed) {
            step = StepResult(stepName, TestStatus::NotEvaluated,
                              QStringLiteral("Keine Seriennummer eingegeben, ZNI meldet \"%1\"")
                                  .arg(response.ascii(2, 29).trimmed()));
            step.setSent(request.toHex());
            step.setReceived(response.toHex());
        }
        result.addStep(step);
        return result;
    }

    result.addStep(sendStep(context, stepName, request, compareSerialNumber));
    return result;
}
