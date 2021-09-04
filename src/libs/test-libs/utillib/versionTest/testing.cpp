/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
    CThisTest(void) : Test() {
    }
    virtual void SetUp(void) {
    }
    virtual void TearDown(void) {
    }
};

//------------------------------------------------------------------------
TEST_F(CThisTest, TestTest_1) {
    cout << "Running " << testName << "\n";

    // ASSERT_EQ("getVersionStr", getVersionStr(), "GHC-TrueBlocks//0.10.3-alpha--git-hash---git-ts-");
    // ASSERT_EQ("getVersionStr", getVersionStr(false), "0.10.3-alpha--git-hash---git-ts-");
    // ASSERT_EQ("getVersionStr", getVersionStr(false, false), "0.10.3-alpha");
    // ASSERT_EQ("getVersionNum", getVersionNum(), 10003);
    ASSERT_EQ("getVersionNum", getVersionNum(0, 9, 5), 9005);
    ASSERT_EQ("getVersionNum", getVersionNum(0, 10, 1), 10001);
    ASSERT_EQ("getVersionNum", getVersionNum(0, 100, 12), 100012);
    ASSERT_EQ("getVersionNum", getVersionNum(0, 1000, 12), 1000012);  // this will conflict
    ASSERT_EQ("getVersionNum", getVersionNum(1, 0, 12), 1000012);

    return true;
}
}

//------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    LOAD_TEST(TestTest_1);
    return testing::RUN_ALL_TESTS();
}
