#include "zlvTest.h"

#include "core/testContext.h"

ZlvTest::ZlvTest()
    : TestCase(QStringLiteral("ZLV-Bus-Tests"))
{
}

ZlvTest::~ZlvTest() = default;

TestResult ZlvTest::run(TestContext &context)
{
    Q_UNUSED(context)

    TestResult result(name());
    // TODO: Prüfschritte als private Methoden ergänzen und je ein StepResult anhängen
    return result;
}
