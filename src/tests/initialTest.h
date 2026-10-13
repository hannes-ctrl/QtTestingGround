#ifndef ZNI_BG_TESTING_INITIALTEST_H
#define ZNI_BG_TESTING_INITIALTEST_H

#include "core/testCase.h"

/**
 * @brief Erste Tests vor allen anderen (Testspezifikation Kapitel 2).
 */
class InitialTest : public TestCase {
public:
    InitialTest();
    ~InitialTest() override;

    TestResult run(TestContext &context) override;
};

#endif //ZNI_BG_TESTING_INITIALTEST_H
