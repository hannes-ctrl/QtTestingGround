#ifndef ZNI_BG_TESTING_TTYTEST_H
#define ZNI_BG_TESTING_TTYTEST_H

#include "core/testCase.h"
#include "hardware/telegramBuilder.h"

/**
 * @brief Tests der TTY-Schnittstellen (Testspezifikation Kapitel 4).
 */
class TtyTest : public TestCase {
public:
    TtyTest();
    ~TtyTest() override;

    TestResult run(TestContext &context) override;

private:
    /**
     * @brief Konfigurieren, Kanal öffnen, Telegramm senden, Kanal schließen für eine TTY-Schnittstelle.
     */
    void testTty(TestContext &context, ZniChannel tty, TestResult &result) const;
};

#endif //ZNI_BG_TESTING_TTYTEST_H
