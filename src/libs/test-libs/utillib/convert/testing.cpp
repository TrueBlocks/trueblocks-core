/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
TEST_F(CThisTest, TestConverts_0) {
    cout << "Running " << testName << "\n";

    CAddressArray array;
    array.push_back("");
    array.push_back("0x");
    array.push_back("0x0");
    array.push_back("0x0000000000000000000000000000000000000000");
    array.push_back("0x1000000000000000000000000000000000000000");

    ASSERT_EQ("empty address",                       array[0], "");
    ASSERT_EQ("hex ony",                             array[1], "0x");
    ASSERT_EQ("zero addr",                           array[2], "0x0");
    ASSERT_EQ("full zero",                           array[3], "0x0000000000000000000000000000000000000000");
    ASSERT_EQ("big num",                             array[4], "0x1000000000000000000000000000000000000000");
    cout << "\n";

    ASSERT_EQ("long empty address",      str_2_Int(array[0]), 0);
    ASSERT_EQ("long hex ony",            str_2_Int(array[1]), 0);
    ASSERT_EQ("long zero addr",          str_2_Int(array[2]), 0);
    ASSERT_EQ("long full zero",          str_2_Int(array[3]), 0);
    ASSERT_EQ("long overflow",           str_2_Int(array[4]), 0);
    cout << "\n";

    ASSERT_EQ("ulong empty address",     str_2_Uint(array[0]), 0);
    ASSERT_EQ("ulong hex ony",           str_2_Uint(array[1]), 0);
    ASSERT_EQ("ulong zero addr",         str_2_Uint(array[2]), 0);
    ASSERT_EQ("ulong full zero",         str_2_Uint(array[3]), 0);
    ASSERT_EQ("ulong overflow",          str_2_Uint(array[4]), 0);
    cout << "\n";

    ASSERT_EQ("str_2_Hash1",             str_2_Hash("12312ABcdE12"),   "0x000000000000000000000000000000000000000000000000000012312abcde12");  // NOLINT
    ASSERT_EQ("str_2_Hash2",             str_2_Hash("a"),              "0x000000000000000000000000000000000000000000000000000000000000000a");  // NOLINT
    ASSERT_EQ("str_2_Hash3",             str_2_Hash("0x"),             "0x0");
    ASSERT_EQ("str_2_Hash4",             str_2_Hash(""),               "0x0");
    ASSERT_EQ("str_2_Hash5",             str_2_Hash("0x12312abcde12"), "0x000000000000000000000000000000000000000000000000000012312abcde12");  // NOLINT
    cout << "\n";

    ASSERT_EQ("str_2_Addr1",             str_2_Addr("12312ABcdE12"),   "0x000000000000000000000000000012312abcde12");
    ASSERT_EQ("str_2_Addr2",             str_2_Addr("a"),              "0x000000000000000000000000000000000000000a");
    ASSERT_EQ("str_2_Addr3",             str_2_Addr("0x"),             "0x0");
    ASSERT_EQ("str_2_Addr4",             str_2_Addr(""),               "0x0");
    ASSERT_EQ("str_2_Addr5",             str_2_Addr("0x12312abcde12"), "0x000000000000000000000000000012312abcde12");
    ASSERT_EQ("str_2_Addr6",             str_2_Addr("000000000000000000000000de12121212312abcde12121212312abcde121212"),"0xde12121212312abcde12121212312abcde121212");
    cout << "\n";

    ASSERT_EQ("timestamp empty address", str_2_Ts(array[0]), 0);
    ASSERT_EQ("timestamp hex ony",       str_2_Ts(array[1]), 0);
    ASSERT_EQ("timestamp zero addr",     str_2_Ts(array[2]), 0);
    ASSERT_EQ("timestamp full zero",     str_2_Ts(array[3]), 0);
    ASSERT_EQ("timestamp overflow",      str_2_Ts(array[4]), 0);
    cout << "\n";

    ASSERT_EQ("bn empty address",        str_2_BigUint(array[0]), 0);
    ASSERT_EQ("bn hex ony",              str_2_BigUint(array[1]), 0);
    ASSERT_EQ("bn zero addr",            str_2_BigUint(array[2]), 0);
    ASSERT_EQ("bn full zero",            str_2_BigUint(array[3]), 0);
    ASSERT_EQ("bn overflow",             str_2_BigUint(array[4]), str_2_Wei("91343852333181432387730302044767688728495783936"));  // NOLINT
    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestConverts_1) {
    cout << "Running " << testName << "\n";

    ASSERT_EQ("chr_2_HexStr1", chr_2_HexStr("This is a test of the emergency broadcast system"),
              "0x5468697320697320612074657374206f662074686520656d657267656e63792062726f6164636173742073797374656d");
    ASSERT_EQ("chr_2_HexStr2", chr_2_HexStr("The quick brown fox jumps over the lazy dog"),
              "0x54686520717569636b2062726f776e20666f78206a756d7073206f76657220746865206c617a7920646f67");
    ASSERT_EQ("chr_2_HexStr3", chr_2_HexStr(""), "0x");
    ASSERT_EQ("chr_2_HexStr4", chr_2_HexStr("0x"), "0x");
    ASSERT_EQ("chr_2_HexStr5", chr_2_HexStr("0x0"), "0x0");
    ASSERT_EQ("chr_2_HexStr6", chr_2_HexStr("0"), "0x30");
    ASSERT_EQ("chr_2_HexStr7", chr_2_HexStr("0x0123456789abcdef"), "0x0123456789abcdef");
    ASSERT_EQ("chr_2_HexStr8", chr_2_HexStr("0xabcdefg"), "0xabcdefg");
    ASSERT_EQ("chr_2_HexStr9", chr_2_HexStr("012345"), "0x303132333435");

    ASSERT_EQ("convert1",  str_2_Double("1"), 1);
    ASSERT_EQ("convert2",  str_2_Double("10000.10001"), 10000.10001);
    ASSERT_EQ("convert3",  str_2_Double("-100101.1212"), -100101.1212);
    ASSERT_EQ("convert4",  str_2_Double(".000000121.4123+e12"), .000000121);
    ASSERT_EQ("convert6",  str_2_Bool("false"), 0);
    ASSERT_EQ("convert7",  str_2_Bool("xxx"), 0);
    ASSERT_EQ("convert8",  str_2_Bool("1"), 1);
    ASSERT_EQ("convert9",  str_2_Bool("0"), 0);
    ASSERT_EQ("convert10", str_2_Bool("338"), 1);
    ASSERT_EQ("convert11", str_2_Bool("true"), 1);

    ASSERT_EQ("bigint1", str_2_BigUint("40000000000000000000"), str_2_Wei("40000000000000000000"));
    ASSERT_EQ("bigint2", str_2_BigInt ("-40000000000000000000"), bigint_t(str_2_BigUint("40000000000000000000"), -1));
    ASSERT_EQ("bigint2", str_2_BigInt ("+3"), 3);
    biguint_t bn = 300000000; bn *= 100000000; bn *= 100;
    ASSERT_EQ("bigint2", str_2_BigInt ("+3000000000000000000"), bn);

    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestConverts_2) {
    cout << "Running " << testName << "\n";

    wei_t one   = 1200000000;
    wei_t two   = 1030000000;
    wei_t three = 1004000001;
    wei_t val = (one * two * three) + 1;

    ASSERT_EQ("wei1", one,   1200000000);
    ASSERT_EQ("wei2", two,   1030000000);
    ASSERT_EQ("wei3", three, 1004000001);
    ASSERT_EQ("wei4", bnu_2_Str(val), "1240944001236000000000000001");
    ASSERT_EQ("eth1", wei_2_Ether(val), "1240944001.236000000000000001");
    ASSERT_EQ("eth2", wei_2_Ether(val/1000000), "1240.944001236");
    ASSERT_EQ("eth2", wei_2_Ether(bnu_2_Str(val)), "1240944001.236000000000000001");
    ASSERT_EQ("eth2", wei_2_Ether(bnu_2_Str(val/1000000)), "1240.944001236000000000");

    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestConverts_3) {
    cout << "Running " << testName << "\n";

    // Block 5,000,000 timestamp: 1517319693 date: 2018-01-30 13:41:33 UTC
    timestamp_t ts = 1517319693;
    ASSERT_EQ("time1", ts, 1517319693);
    ASSERT_EQ("time2", str_2_Ts("1517319693"), 1517319693);
    ASSERT_EQ("time2", ts_2_Str(1517319693), "1517319693");
    time_q date = ts_2_Date(ts);
    ASSERT_EQ("time3", date, time_q(2018, 01, 30, 13, 41, 33));
    ASSERT_EQ("time4", date_2_Ts(date), ts);

    ASSERT_EQ("time5", str_2_Bool("1"), true);
    ASSERT_EQ("time6", str_2_Hex("1212"), "0x4bc");
    ASSERT_EQ("time7", str_2_Gas("4000000000"), 4000000000);
    uint64_t x = 4000000000;
    wei_t wei = x;
    wei *= x;
    ASSERT_EQ("time8", str_2_Wei("16000000000000000000"), wei);
    ASSERT_EQ("time9", str_2_Bloom("16000000000000000000"), wei);
    ASSERT_EQ("timeA", str_2_Topic("16000000000000000000"), wei);

    return true;
}}

#include "options.h"
//------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    CStringArray commands;
    explode(commands, options.commandList, '\n');
    for (auto command : commands) {
        if (!options.parseArguments(command))
            return 0;

        switch (options.testNum) {
            case 0: LOAD_TEST(TestConverts_0); break;
            case 1: LOAD_TEST(TestConverts_1); break;
            case 2: LOAD_TEST(TestConverts_2); break;
            case 3: LOAD_TEST(TestConverts_3); break;
        }
    }

    return testing::RUN_ALL_TESTS();
}

#if 0
toTimeStamp
to_int
to_long
to_uint
to_ulong
#endif
