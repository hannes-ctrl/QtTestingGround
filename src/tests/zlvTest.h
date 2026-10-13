#ifndef ZNI_BG_TESTING_ZLVTEST_H
#define ZNI_BG_TESTING_ZLVTEST_H

#include "core/testCase.h"
#include "hardware/telegramBuilder.h"

/**
 * @brief Tests des ZLV-Busses (Testspezifikation Kapitel 3).
 */
class ZlvTest : public TestCase {
public:
    ZlvTest();
    ~ZlvTest() override;

    TestResult run(TestContext &context) override;

private:
    /**
     * @brief Kanal öffnen, ZLV-Telegramm senden, Kanal schließen für ein Modem.
     */
    void testModem(TestContext &context, ZniChannel modem, TestResult &result) const;
};

#endif //ZNI_BG_TESTING_ZLVTEST_H
