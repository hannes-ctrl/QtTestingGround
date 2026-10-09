#ifndef ZNI_BG_TESTING_TESTRESULT_H
#define ZNI_BG_TESTING_TESTRESULT_H

#include <QMetaType>
#include <QString>
#include <QVector>

/**
 * @brief Status eines Tests bzw. eines einzelnen Prüfschritts.
 */
enum class TestStatus {
    NotRun,
    Passed,
    Failed,
    Skipped,
    Error
};

/**
 * @brief Ergebnis eines einzelnen Prüfschritts innerhalb eines Tests.
 */
class StepResult {
public:
    StepResult();
    StepResult(const QString &name, TestStatus status, const QString &message = QString());
    ~StepResult();

    QString name() const;
    TestStatus status() const;
    QString message() const;

private:
    QString m_name;
    TestStatus m_status;
    QString m_message;
};

/**
 * @brief Ergebnis eines Tests, bestehend aus den Ergebnissen seiner Prüfschritte.
 */
class TestResult {
public:
    TestResult();
    explicit TestResult(const QString &testName);
    ~TestResult();

    void addStep(const StepResult &step);

    QString testName() const;
    QVector<StepResult> steps() const;

    /**
     * @brief Gesamtstatus, abgeleitet aus den Prüfschritten.
     */
    TestStatus status() const;

    qint64 durationMs() const;
    void setDurationMs(qint64 durationMs);

private:
    QString m_testName;
    QVector<StepResult> m_steps;
    qint64 m_durationMs;
};

Q_DECLARE_METATYPE(TestResult)

#endif //ZNI_BG_TESTING_TESTRESULT_H
