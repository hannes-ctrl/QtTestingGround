#ifndef ZNI_BG_TESTING_SWITCHCARDSTEST_H
#define ZNI_BG_TESTING_SWITCHCARDSTEST_H

#include "core/testCase.h"

/**
 * @brief Tests der Anschaltkarten.
 */
class SwitchCardsTest : public TestCase {
public:
    SwitchCardsTest();
    ~SwitchCardsTest() override;

    TestResult run(TestContext &context) override;
};

#endif //ZNI_BG_TESTING_SWITCHCARDSTEST_H
