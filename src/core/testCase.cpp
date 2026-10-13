#include "testCase.h"

#include "hardware/telegram.h"
#include "hardware/zniConnection.h"
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
                              Telegram *response) const
{
    ZniConnection *connection = context.connection();

    Telegram received;
    if (!connection->send(request, received)) {
        StepResult step(stepName, TestStatus::Error, connection->errorString());
        step.setSent(request.toHex());
        return step;
    }

    // TODO: Antwort auswerten (Fehlercode über ZniErrorCode, Soll-Ist-Vergleich), sobald der Aufbau
    //       der Antworttelegramme feststeht. Bis dahin wird der Schritt nicht bewertet.
    StepResult step(stepName, TestStatus::NotEvaluated, QStringLiteral("Antwort empfangen, noch nicht ausgewertet"));
    step.setSent(request.toHex());
    step.setReceived(received.toHex());

    if (response != nullptr) {
        *response = received;
    }
    return step;
}

StepResult TestCase::skippedStep(const QString &stepName, const QString &reason)
{
    return StepResult(stepName, TestStatus::Skipped, reason);
}
