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

//------------------------------------------------------------------------
TEST_F(CThisTest, Test01) {

    cerr << "Running " << testName << "\n";

    uint64_t val64u = uint64_t(-1);
    string_q sValu = uint_2_Str(val64u);
    int64_t val64 = -1;
    string_q sVal = int_2_Str(val64);

    cout << "val64u: " << val64u << "\n";
    cout << "sValu: "  << sValu  << "\n";
    cout << "val64: "  << val64  << "\n";
    cout << "sVal: "   << sVal   << "\n";

    ASSERT_EQ("Testing 1",          val64u, str_2_Uint(sValu));
    ASSERT_EQ("Testing 2",          val64,  str_2_Int(sVal));
    ASSERT_EQ("Testing 3", (int64_t)val64u, -1);
    ASSERT_EQ("Testing 4",          val64,  -1);

    return true;
}}

#include "options.h"
//------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
    }

    LOAD_TEST(Test01);
    return testing::RUN_ALL_TESTS();
}
