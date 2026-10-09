#ifndef ZNI_BG_TESTING_TTYTEST_H
#define ZNI_BG_TESTING_TTYTEST_H

#include "core/testCase.h"

/**
 * @brief Tests der TTY-Schnittstellen.
 */
class TtyTest : public TestCase {
public:
    TtyTest();
    ~TtyTest() override;

    TestResult run(TestContext &context) override;
};

#endif //ZNI_BG_TESTING_TTYTEST_H
