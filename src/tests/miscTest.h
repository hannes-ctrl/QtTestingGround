#ifndef ZNI_BG_TESTING_MISCTEST_H
#define ZNI_BG_TESTING_MISCTEST_H

#include "core/testCase.h"

/**
 * @brief Tests der sonstigen Schnittstellen: Status und Reset (Testspezifikation Kapitel 7).
 *
 * Läuft als letzter Test, weil der Reset am Ende steht.
 */
class MiscTest : public TestCase {
public:
    MiscTest();
    ~MiscTest() override;

    TestResult run(TestContext &context) override;
};

#endif //ZNI_BG_TESTING_MISCTEST_H
