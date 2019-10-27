/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <string>
#include <algorithm>
#include "etherlib.h"
#include "options.h"
#include "testing.h"

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
    if (opts.testNum ==  8) ASSERT_TRUE( "8. uint == -100 (false)", (opts.numOption == (int64_t)NOPOS) );
    if (opts.testNum ==  9) ASSERT_TRUE( "9. string with string",   (opts.stringOption == "This is a string") );
    if (opts.testNum == 10) ASSERT_TRUE("10. string with 1212",     (opts.stringOption == "1212") );

    return true;
}}

//------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    opts.minArgs = 0;
    if (!opts.prepareArguments(argc, argv))
        return 0;

    bool ret = true;
    LOAD_TEST(Test1);
    for (auto command : opts.commandLines) {
        if (!opts.parseArguments(command))
            return 0;
        cout << string_q(80, '-') << "\n";
        opts.output();
        ret &= testing::RUN_ALL_TESTS();
    }
    cout << "ret: " << ret << "\n";
    return ret;  // if all tests passes, return '0'
}
