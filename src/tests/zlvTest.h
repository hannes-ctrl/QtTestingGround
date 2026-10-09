#ifndef ZNI_BG_TESTING_ZLVTEST_H
#define ZNI_BG_TESTING_ZLVTEST_H

#include "core/testCase.h"

/**
 * @brief Tests des ZLV-Busses.
 */
class ZlvTest : public TestCase {
public:
    ZlvTest();
    ~ZlvTest() override;

    TestResult run(TestContext &context) override;
};

#endif //ZNI_BG_TESTING_ZLVTEST_H
