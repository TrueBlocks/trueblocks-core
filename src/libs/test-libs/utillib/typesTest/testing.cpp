/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
            return 0;
    }

    LOAD_TEST(Test01);
    return RUN_ALL_TESTS();
}
