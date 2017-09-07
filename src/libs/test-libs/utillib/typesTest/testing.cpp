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
TEST_F(CThisTest, Test01) {

    cerr << "Running " << testName << "\n";

    uint64_t val64u = uint64_t(-1);
    SFString sValu = asStringU(val64u);
    int64_t val64 = -1;
    SFString sVal = asString(val64);

    cout << "val64u: " << val64u << "\n";
    cout << "sValu: "  << sValu  << "\n";
    cout << "val64: "  << val64  << "\n";
    cout << "sVal: "   << sVal   << "\n";

    ASSERT_EQ("Testing 1",          val64u, toLongU(sValu));
    ASSERT_EQ("Testing 2",          val64,  toLong(sVal));
    ASSERT_EQ("Testing 3", (int64_t)val64u, -1);
    ASSERT_EQ("Testing 4",          val64,  -1);

    return true;
}}

#include "options.h"
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
    }

    LOAD_TEST(Test01);
    return RUN_ALL_TESTS();
}
