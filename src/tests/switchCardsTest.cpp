#include "switchCardsTest.h"

#include "core/testContext.h"

SwitchCardsTest::SwitchCardsTest()
    : TestCase(QStringLiteral("Anschaltkarten-Tests"))
{
}

SwitchCardsTest::~SwitchCardsTest() = default;

TestResult SwitchCardsTest::run(TestContext &context)
{
    Q_UNUSED(context)

    TestResult result(name());
    // TODO: Prüfschritte als private Methoden ergänzen und je ein StepResult anhängen
    return result;
}
