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
#include "utillib.h"
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
TEST_F(CThisTest, TestInsertion) {
    CStringArray strArray;
    strArray.push_back("c-string");
    strArray.push_back(string_q("string_q"));
    string str = "string";
    strArray.push_back(str.c_str());
    strArray.push_back("When will we be loved?");
    for (auto elem : strArray)
        cout << TESTID("array ", 20) << elem << "\n";

    CUintArray myvector;
    for (size_t i = 0; i < 10; i++)
        myvector.push_back(i);
    cout << TESTID("int array", 20);
    for (size_t i = 0; i < 10; i++)
        cout << ' ' << myvector[i];
    cout << '\n';
    for (size_t i = 0; i < myvector.size() / 2; i++) {
        uint64_t temp;
        temp = myvector[myvector.size() - 1 - i];
        myvector.at(myvector.size() - 1 - i) = myvector[i];
        myvector.at(i) = temp;
    }
    cout << TESTID("reversed", 20);
    for (size_t i = 0; i < 10; i++)
        cout << ' ' << myvector[i];
    cout << '\n';

    return true;
}
}

#include "options.h"
//------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        LOAD_TEST(TestInsertion);
    }

    return testing::RUN_ALL_TESTS();
}
