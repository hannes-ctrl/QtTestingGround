#include "testResult.h"

QString testStatusToString(const TestStatus status)
{
    switch (status) {
    case TestStatus::NotRun:
        return QStringLiteral("notRun");
    case TestStatus::Passed:
        return QStringLiteral("passed");
    case TestStatus::Failed:
        return QStringLiteral("failed");
    case TestStatus::Skipped:
        return QStringLiteral("skipped");
    case TestStatus::Error:
        return QStringLiteral("error");
    }
    return QString();
}

StepResult::StepResult()
    : m_status(TestStatus::NotRun)
{
}

StepResult::StepResult(const QString &name, const TestStatus status, const QString &message)
    : m_name(name)
    , m_status(status)
    , m_message(message)
{
}

StepResult::~StepResult() = default;

QString StepResult::name() const
{
    return m_name;
}

TestStatus StepResult::status() const
{
    return m_status;
}

QString StepResult::message() const
{
    return m_message;
}

QString StepResult::sent() const
{
    return m_sent;
}

void StepResult::setSent(const QString &sent)
{
    m_sent = sent;
}

QString StepResult::received() const
{
    return m_received;
}

void StepResult::setReceived(const QString &received)
{
    m_received = received;
}

QString StepResult::expected() const
{
    return m_expected;
}

void StepResult::setExpected(const QString &expected)
{
    m_expected = expected;
}

TestResult::TestResult()
    : m_durationMs(0)
{
}

TestResult::TestResult(const QString &testName)
    : m_testName(testName)
    , m_durationMs(0)
{
}

TestResult::~TestResult() = default;

void TestResult::addStep(const StepResult &step)
{
    m_steps.append(step);
}

QString TestResult::testName() const
{
    return m_testName;
}

QVector<StepResult> TestResult::steps() const
{
    return m_steps;
}

TestStatus TestResult::status() const
{
    if (m_steps.isEmpty()) {
        return TestStatus::NotRun;
    }

    TestStatus overall = TestStatus::Passed;
    for (const StepResult &step : m_steps) {
        if (step.status() == TestStatus::Error) {
            return TestStatus::Error;
        }
        if (step.status() == TestStatus::Failed) {
            overall = TestStatus::Failed;
        }
    }
    return overall;
}

qint64 TestResult::durationMs() const
{
    return m_durationMs;
}

void TestResult::setDurationMs(const qint64 durationMs)
{
    m_durationMs = durationMs;
}
