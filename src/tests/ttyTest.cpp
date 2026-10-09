#include "ttyTest.h"

#include "core/testContext.h"

TtyTest::TtyTest()
    : TestCase(QStringLiteral("TTY-Schnittstellen-Tests"))
{
}

TtyTest::~TtyTest() = default;

TestResult TtyTest::run(TestContext &context)
{
    Q_UNUSED(context)

    TestResult result(name());
    // TODO: Prüfschritte als private Methoden ergänzen und je ein StepResult anhängen
    return result;
}
