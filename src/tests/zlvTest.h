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
     * @brief Konfigurieren, Kanal öffnen, ZLV-Status abwarten, ZLV-Telegramm senden, Kanal schließen.
     */
    void testModem(TestContext &context, ZniChannel modem, TestResult &result) const;

    /**
     * @brief Wartet auf die zyklische Statusmeldung (Telegramm 62) des Modems und bewertet den Bus-Status.
     */
    StepResult receiveZlvStatus(TestContext &context, ZniChannel modem) const;
};

#endif //ZNI_BG_TESTING_ZLVTEST_H
