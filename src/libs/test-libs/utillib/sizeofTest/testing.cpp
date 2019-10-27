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
                 CThisTest(void) : Test() {}
    virtual void SetUp    (void) {}
    virtual void TearDown (void) {}
};

//------------------------------------------------------------------------
TEST_F(CThisTest, TestSizes) {

    cout << "Running " << testName << "\n";

    cout << TESTID("sizeof(char)",     20) << sizeof(char)      << "\n";
    cout << TESTID("sizeof(int)",      20) << sizeof(int)       << "\n";
    cout << TESTID("sizeof(short)",    20) << sizeof(short)     << "\n";  // NOLINT
    cout << TESTID("sizeof(long)",     20) << sizeof(long)      << "\n";  // NOLINT
    cout << TESTID("sizeof(unsigned)", 20) << sizeof(unsigned)  << "\n";  // NOLINT
    cout << TESTID("sizeof(size_t)",   20) << sizeof(size_t)    << "\n";
    cout << TESTID("sizeof(uint32_t)", 20) << sizeof(uint32_t)  << "\n";
    cout << TESTID("sizeof(uint64_t)", 20) << sizeof(uint64_t)  << "\n";
    cout << "\n";

    cout << TESTID("sizeof(bool)",               20) << sizeof(bool)               << "\n";
    cout << TESTID("sizeof(char)",               20) << sizeof(char)               << "\n";
    cout << TESTID("sizeof(int)",                20) << sizeof(int)                << "\n";
    cout << TESTID("sizeof(unsigned int)",       20) << sizeof(unsigned int)       << "\n";
    cout << TESTID("sizeof(long)",               20) << sizeof(long)               << "\n";  // NOLINT
    cout << TESTID("sizeof(unsigned long)",      20) << sizeof(unsigned long)      << "\n";  // NOLINT
    cout << TESTID("sizeof(long long)",          20) << sizeof(long long)          << "\n";  // NOLINT
    cout << TESTID("sizeof(unsigned long long)", 20) << sizeof(unsigned long long) << "\n";  // NOLINT
    cout << TESTID("sizeof(float)",              20) << sizeof(float)              << "\n";
    cout << TESTID("sizeof(double)",             20) << sizeof(double)             << "\n";
    cout << TESTID("sizeof(const char *)",       20) << sizeof(const char *)       << "\n";

    return true;
}}

#include "options.h"
//------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
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
