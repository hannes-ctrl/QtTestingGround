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
    Error,
    /// Ausgeführt und Antwort erhalten, aber noch ohne Soll-Ist-Vergleich
    NotEvaluated
};

/**
 * @brief Maschinenlesbarer Name eines Status, wie er im JSON-Protokoll steht (z. B. "passed").
 */
QString testStatusToString(TestStatus status);

/**
 * @brief Fasst mehrere Status zu einem Gesamtstatus zusammen.
 *
 * Es gewinnt der schwerwiegendste: Error, dann Failed, dann NotEvaluated, dann Passed.
 * Skipped und NotRun zählen nur, wenn nichts anderes vorliegt.
 */
TestStatus combineTestStatus(const QVector<TestStatus> &statuses);

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

    /**
     * @brief Gesendetes, empfangenes und erwartetes Telegramm als Hex-Text (leer, wenn nicht zutreffend).
     */
    QString sent() const;
    void setSent(const QString &sent);
    QString received() const;
    void setReceived(const QString &received);
    QString expected() const;
    void setExpected(const QString &expected);

private:
    QString m_name;
    TestStatus m_status;
    QString m_message;
    QString m_sent;
    QString m_received;
    QString m_expected;
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
