#include "initialTest.h"

#include "core/testContext.h"
#include "hardware/telegramBuilder.h"

InitialTest::InitialTest()
    : TestCase(QStringLiteral("Erste Tests"))
{
}

InitialTest::~InitialTest() = default;

TestResult InitialTest::run(TestContext &context)
{
    TestResult result(name());

    // TODO: Seriennummer aus der Antwort lesen und mit context.serialNumber() vergleichen,
    //       sobald der Aufbau der Antwort feststeht
    result.addStep(sendStep(context, QStringLiteral("Seriennummer abfragen"),
                            TelegramBuilder::status(TelegramBuilder::StatusPageSerialNumber)));
    return result;
}
