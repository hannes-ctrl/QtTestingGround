#include "testRunner.h"

#include <QElapsedTimer>

#include "testContext.h"

TestRunner::TestRunner(QObject *parent)
    : QObject(parent)
    , m_context(nullptr)
{
}

TestRunner::~TestRunner() = default;

void TestRunner::setTests(std::vector<std::unique_ptr<TestCase>> tests)
{
    m_tests = std::move(tests);
}

void TestRunner::setContext(TestContext *context)
{
    m_context = context;
}

void TestRunner::abort()
{
    if (m_context != nullptr) {
        m_context->setAbortRequested(true);
    }
}

void TestRunner::run()
{
    if (m_context == nullptr) {
        emit runFinished();
        return;
    }

    m_context->setAbortRequested(false);

    for (const std::unique_ptr<TestCase> &test : m_tests) {
        if (m_context->abortRequested()) {
            break;
        }

        emit testStarted(test->name());

        QElapsedTimer timer;
        timer.start();
        TestResult result = test->run(*m_context);
        result.setDurationMs(timer.elapsed());

        emit testFinished(result);
    }

    m_tests.clear();
    emit runFinished();
}
