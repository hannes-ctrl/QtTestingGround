#include "testSession.h"

TestSession::TestSession(const QString &serialNumber)
    : m_serialNumber(serialNumber)
    , m_startedAt(QDateTime::currentDateTime())
{
}

TestSession::~TestSession() = default;

void TestSession::addResult(const TestResult &result)
{
    m_results.append(result);
}

void TestSession::finish()
{
    m_finishedAt = QDateTime::currentDateTime();
}

QString TestSession::serialNumber() const
{
    return m_serialNumber;
}

QDateTime TestSession::startedAt() const
{
    return m_startedAt;
}

QDateTime TestSession::finishedAt() const
{
    return m_finishedAt;
}

QVector<TestResult> TestSession::results() const
{
    return m_results;
}

TestStatus TestSession::status() const
{
    QVector<TestStatus> statuses;
    for (const TestResult &result : m_results) {
        statuses.append(result.status());
    }
    return combineTestStatus(statuses);
}
