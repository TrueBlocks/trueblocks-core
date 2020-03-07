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
    CThisTest(void) : Test() {
    }
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
    cout << "sValu: " << sValu << "\n";
    cout << "val64: " << val64 << "\n";
    cout << "sVal: " << sVal << "\n";

    ASSERT_EQ("Testing 1", val64u, str_2_Uint(sValu));
    ASSERT_EQ("Testing 2", val64, str_2_Int(sVal));
    ASSERT_EQ("Testing 3", (int64_t)val64u, -1);
    ASSERT_EQ("Testing 4", val64, -1);

    return true;
}
}

//------------------------------------------------------------------------
TEST_F(CThisTest, Test02) {
    cerr << "Running " << testName << "\n";

#define TT(a)                                                                                                          \
    {                                                                                                                  \
        cout                                                                                                           \
            << "\t" << padRight(#a + string_q(":"), 14)                                                                \
            << substitute(                                                                                             \
                   bloom_2_Bytes(a),                                                                                   \
                   "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" \
                   "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" \
                   "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" \
                   "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" \
                   "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",   \
                   "0x")                                                                                               \
            << endl;                                                                                                   \
    }

    cout << "Attributes" << endl;
    TT(TS_NUMERAL)
    TT(TS_STRING)
    TT(TS_DATE)
    TT(TS_ARRAY)
    TT(TS_OBJECT)
    TT(TS_POINTER)
    TT(TS_BIGNUM)
    TT(TS_INTEGER)
    cout << "Types" << endl;
    TT(T_DATE)
    TT(T_TIME)
    TT(T_TIMESTAMP)
    TT(T_BOOL)
    TT(T_NUMBER)
    TT(T_UNUMBER)
    TT(T_BLOCKNUM)
    TT(T_DOUBLE)
    TT(T_WEI)
    TT(T_GAS)
    TT(T_ETHER)
    TT(T_UINT256)
    TT(T_INT256)
    TT(T_TEXT)
    TT(T_ADDRESS)
    TT(T_HASH)
    TT(T_IPFSHASH)
    TT(T_BLOOM)
    TT(T_POINTER)
    TT(T_OBJECT)
    ASSERT_EQ("Types", 1, 1);

    return true;
}
}

#include "options.h"
//------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
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
    LOAD_TEST(Test02);
    return testing::RUN_ALL_TESTS();
}
