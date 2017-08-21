/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include <string>
#include <algorithm>
#include "etherlib.h"
#include "options.h"

// Changing this between qstring and SFString helps migrating away from quickBlocks code
#define TEST_STR SFString

//------------------------------------------------------------------------
class CThisTest : public testing::Test {
public:
    CThisTest(void) : Test() {}
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
};

//------------------------------------------------------------------------
TEST_F(CThisTest, Test1) {

    cerr << "Running " << testName << "\n";

    ASSERT_EQ    ("equal",     1, 1);
    ASSERT_NOT_EQ("not equal", 1, 2);
    ASSERT_TRUE  ("true",      (1 == 1));
    ASSERT_FALSE ("false",     (1 == 2));

    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, Test2) {
    
    cerr << "Running " << testName << "\n";
    
    ASSERT_EQ    ("equal",     1, 1);
    ASSERT_NOT_EQ("not equal", 1, 2);
    ASSERT_TRUE  ("true",      (1 == 1));
    ASSERT_FALSE ("false",     (1 == 2));
    
    return true;
}}

//------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    COptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return false;
        LOAD_TEST(Test1);
        LOAD_TEST(Test2);
    }

    return RUN_ALL_TESTS();
}
