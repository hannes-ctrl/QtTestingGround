#include "miscTest.h"

#include "core/testContext.h"
#include "hardware/telegramBuilder.h"

MiscTest::MiscTest()
    : TestCase(QStringLiteral("Sonstige Tests"))
{
}

MiscTest::~MiscTest() = default;

TestResult MiscTest::run(TestContext &context)
{
    TestResult result(name());

    result.addStep(sendStep(context, QStringLiteral("ZNI-Status abfragen"),
                            TelegramBuilder::status(TelegramBuilder::StatusPageGeneral)));

    if (context.abortRequested()) {
        return result;
    }

    result.addStep(sendStep(context, QStringLiteral("Reset"), TelegramBuilder::reset()));
    return result;
}
