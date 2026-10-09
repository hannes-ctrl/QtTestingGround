#ifndef ZNI_BG_TESTING_TESTRUNNER_H
#define ZNI_BG_TESTING_TESTRUNNER_H

#include <QObject>

#include <memory>
#include <vector>

#include "testCase.h"
#include "testResult.h"

class TestContext;

/**
 * @brief Führt die ausgewählten Tests nacheinander aus. Läuft in einem eigenen Thread,
 *        damit die GUI währenddessen bedienbar bleibt.
 */
class TestRunner : public QObject
{
    Q_OBJECT

    public:
        explicit TestRunner(QObject *parent = nullptr);
        ~TestRunner() override;

        void setTests(std::vector<std::unique_ptr<TestCase>> tests);
        void setContext(TestContext *context);

        /**
         * @brief Fordert den Abbruch an; der laufende Prüfschritt wird noch beendet.
         */
        void abort();

    public slots:
        void run();

    signals:
        void testStarted(const QString &testName);
        void testFinished(const TestResult &result);
        void runFinished();

    private:
        std::vector<std::unique_ptr<TestCase>> m_tests;
        TestContext *m_context;
};

#endif //ZNI_BG_TESTING_TESTRUNNER_H
