#include "testCase.h"

#include "hardware/telegram.h"
#include "hardware/zniConnection.h"
#include "hardware/zniErrorCode.h"
#include "testContext.h"

TestCase::TestCase(const QString &name)
    : m_name(name)
{
}

TestCase::~TestCase() = default;

QString TestCase::name() const
{
    return m_name;
}

StepResult TestCase::sendStep(TestContext &context, const QString &stepName, const Telegram &request,
                              const ResponseCheck &check, Telegram *response) const
{
    ZniConnection *connection = context.connection();

    Telegram received;
    if (!connection->send(request, received)) {
        StepResult step(stepName, TestStatus::Error, connection->errorString());
        step.setSent(request.toHex());
        return step;
    }
    if (response != nullptr) {
        *response = received;
    }

    TestStatus status = TestStatus::Passed;
    QString message;
    if (received.errorCode() != 0x00) {
        status = TestStatus::Failed;
        message = QStringLiteral("Fehlercode %1: %2")
                      .arg(hexByte(received.errorCode()), ZniErrorCode::description(received.errorCode()));
    } else if (check && !check(received, message)) {
        status = TestStatus::Failed;
    }

    StepResult step(stepName, status, message);
    step.setSent(request.toHex());
    step.setReceived(received.toHex());
    return step;
}

TestCase::ResponseCheck TestCase::echoesThirdByte(const quint8 expected)
{
    return [expected](const Telegram &response, QString &message) {
        if (response.byteAt(2) != expected) {
            message = QStringLiteral("Antwort meldet Peripherie %1 statt %2")
                          .arg(hexByte(response.byteAt(2)), hexByte(expected));
            return false;
        }
        return true;
    };
}

StepResult TestCase::skippedStep(const QString &stepName, const QString &reason)
{
    return StepResult(stepName, TestStatus::Skipped, reason);
}

QString TestCase::hexByte(const quint8 value)
{
    return QStringLiteral("%1").arg(value, 2, 16, QLatin1Char('0')).toUpper();
}
