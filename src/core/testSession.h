#ifndef ZNI_BG_TESTING_TESTSESSION_H
#define ZNI_BG_TESTING_TESTSESSION_H

#include <QDateTime>
#include <QString>
#include <QVector>

#include "testResult.h"

/**
 * @brief Ein Prüfdurchlauf für eine Baugruppe: Seriennummer, Zeitpunkt und alle Ergebnisse.
 */
class TestSession {
public:
    explicit TestSession(const QString &serialNumber);
    ~TestSession();

    void addResult(const TestResult &result);

    /**
     * @brief Schließt den Durchlauf ab und setzt den Endzeitpunkt.
     */
    void finish();

    QString serialNumber() const;
    QDateTime startedAt() const;
    QDateTime finishedAt() const;
    QVector<TestResult> results() const;

    /**
     * @brief Gesamtstatus über alle Tests des Durchlaufs.
     */
    TestStatus status() const;

private:
    QString m_serialNumber;
    QDateTime m_startedAt;
    QDateTime m_finishedAt;
    QVector<TestResult> m_results;
};

#endif //ZNI_BG_TESTING_TESTSESSION_H
