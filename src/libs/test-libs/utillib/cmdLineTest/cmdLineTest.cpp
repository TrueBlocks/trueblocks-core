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

COptions opts;
//------------------------------------------------------------------------
TEST_F(ThisTest, Test1) {

    if (opts.testNum < 1 || opts.testNum > 10)
        return true;
    
    cerr << "Running " << testName << "\n";

    if (opts.testNum ==  1) ASSERT_TRUE( "1. boolOption == false",  (opts.boolSet && !opts.boolOption) );
    if (opts.testNum ==  2) ASSERT_TRUE( "2. boolOption == true",   (opts.boolSet &&  opts.boolOption) );
    if (opts.testNum ==  3) ASSERT_TRUE( "3. boolOption == false",  (opts.boolSet && !opts.boolOption) );
    if (opts.testNum ==  4) ASSERT_TRUE( "4. boolOption == true",   (opts.boolSet &&  opts.boolOption) );
    if (opts.testNum ==  5) ASSERT_TRUE( "5. int  == 12",           (opts.numOption ==   12) );
    if (opts.testNum ==  6) ASSERT_TRUE( "6. int  == -100",         (opts.numOption == -100) );
    if (opts.testNum ==  7) ASSERT_TRUE( "7. uint == 12",           (opts.numOption ==   12) );
    if (opts.testNum ==  8) ASSERT_TRUE( "8. uint == -100 (false)", (opts.numOption == NOPOS) );
    if (opts.testNum ==  9) ASSERT_TRUE( "9. string with string",   (opts.stringOption == "This is a string") );
    if (opts.testNum == 10) ASSERT_TRUE("10. string with 1212",     (opts.stringOption == "1212") );

    return true;
}}

//------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    opts.minArgs = 0;
    if (!opts.prepareArguments(argc, argv))
        return 0;
    
    bool ret = true;
    LOAD_TEST(Test1);
    while (!opts.commandList.empty()) {
        SFString command = nextTokenClear(opts.commandList, '\n');
        if (!opts.parseArguments(command))
            return false;
        cout << SFString('-',80) << "\n";
        opts.output();
        ret &= RUN_ALL_TESTS();
    }
    cout << "ret: " << ret << "\n";
    return ret;  // if all tests passes, return '0'
}
