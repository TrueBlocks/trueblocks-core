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
TEST_F(CThisTest, TestSizes) {
    cout << "Running " << testName << endl;

    cout << TESTID("sizeof(char)", 20) << sizeof(char) << " : " << (sizeof(char) * 8) << endl;
    cout << TESTID("sizeof(int)", 20) << sizeof(int) << " : " << (sizeof(int) * 8) << endl;
    cout << TESTID("sizeof(short)", 20) << sizeof(short) << " : " << (sizeof(short) * 8) << endl;  // NOLINT
    cout << TESTID("sizeof(long)", 20) << sizeof(long) << " : " << (sizeof(long) * 8) << endl;     // NOLINT
    cout << TESTID("sizeof(unsigned)", 20) << sizeof(unsigned) << " : " << (sizeof(unsigned) * 8) << endl;
    cout << TESTID("sizeof(size_t)", 20) << sizeof(size_t) << " : " << (sizeof(size_t) * 8) << endl;
    cout << TESTID("sizeof(uint32_t)", 20) << sizeof(uint32_t) << " : " << (sizeof(uint32_t) * 8) << endl;
    cout << TESTID("sizeof(uint64_t)", 20) << sizeof(uint64_t) << " : " << (sizeof(uint64_t) * 8) << endl;
    cout << endl;

    cout << TESTID("sizeof(bool)", 20) << sizeof(bool) << " : " << (sizeof(bool) * 8) << endl;
    cout << TESTID("sizeof(char)", 20) << sizeof(char) << " : " << (sizeof(char) * 8) << endl;
    cout << TESTID("sizeof(int)", 20) << sizeof(int) << " : " << (sizeof(int) * 8) << endl;
    cout << TESTID("sizeof(unsigned int)", 20) << sizeof(unsigned int) << " : " << (sizeof(unsigned int) * 8) << endl;
    cout << TESTID("sizeof(long)", 20) << sizeof(long) << " : " << (sizeof(long) * 8) << endl;  // NOLINT
    cout << TESTID("sizeof(unsigned long)", 20) << sizeof(unsigned long) << " : "               // NOLINT
         << (sizeof(unsigned long) * 8)                                                         // NOLINT
         << endl;
    cout << TESTID("sizeof(long long)", 20) << sizeof(long long) << " : " << (sizeof(long long) * 8) << endl;  // NOLINT
    cout << TESTID("sizeof(unsigned long long)", 20) << sizeof(unsigned long long) << " : "                    // NOLINT
         << (sizeof(unsigned long long) * 8) << endl;                                                          // NOLINT
    cout << TESTID("sizeof(float)", 20) << sizeof(float) << " : " << (sizeof(float) * 8) << endl;
    cout << TESTID("sizeof(double)", 20) << sizeof(double) << " : " << (sizeof(double) * 8) << endl;
    cout << TESTID("sizeof(const char *)", 20) << sizeof(const char*) << " : " << (sizeof(const char*) * 8) << endl;

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
        LOAD_TEST(TestSizes);
    }

    return testing::RUN_ALL_TESTS();
}
