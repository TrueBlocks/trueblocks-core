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

    SFAddressArray array;
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

    ASSERT_EQ("toHash1",                 toHash("12312ABcdE12"),   "0x000000000000000000000000000000000000000000000000000012312abcde12");
    ASSERT_EQ("toHash2",                 toHash("a"),              "0x000000000000000000000000000000000000000000000000000000000000000a");
    ASSERT_EQ("toHash3",                 toHash("0x"),             "0x0000000000000000000000000000000000000000000000000000000000000000");
    ASSERT_EQ("toHash4",                 toHash(""),               "0x0000000000000000000000000000000000000000000000000000000000000000");
    ASSERT_EQ("toHash5",                 toHash("0x12312abcde12"), "0x000000000000000000000000000000000000000000000000000012312abcde12");
    cout << "\n";

    ASSERT_EQ("toAddress1",              toAddress("12312ABcdE12"),   "0x000000000000000000000000000012312abcde12");
    ASSERT_EQ("toAddress2",              toAddress("a"),              "0x000000000000000000000000000000000000000a");
    ASSERT_EQ("toAddress3",              toAddress("0x"),             "0x0");
    ASSERT_EQ("toAddress4",              toAddress(""),               "0x0");
    ASSERT_EQ("toAddress5",              toAddress("0x12312abcde12"), "0x000000000000000000000000000012312abcde12");
    cout << "\n";

    ASSERT_EQ("timestamp empty address", toTimestamp(array[0]), 0);
    ASSERT_EQ("timestamp hex ony",       toTimestamp(array[1]), 0);
    ASSERT_EQ("timestamp zero addr",     toTimestamp(array[2]), 0);
    ASSERT_EQ("timestamp full zero",     toTimestamp(array[3]), 0);
    ASSERT_EQ("timestamp overflow",      toTimestamp(array[4]), 0);
    cout << "\n";

    ASSERT_EQ("bn empty address",        str_2_BigUint(array[0]), 0);
    ASSERT_EQ("bn hex ony",              str_2_BigUint(array[1]), 0);
    ASSERT_EQ("bn zero addr",            str_2_BigUint(array[2]), 0);
    ASSERT_EQ("bn full zero",            str_2_BigUint(array[3]), 0);
    ASSERT_EQ("bn overflow",             str_2_BigUint(array[4]), canonicalWei("91343852333181432387730302044767688728495783936"));

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

    ASSERT_EQ("bigint1", str_2_BigUint("40000000000000000000"), canonicalWei("40000000000000000000"));
    ASSERT_EQ("bigint2", str_2_BigInt ("-40000000000000000000"), SFIntBN(str_2_BigUint("40000000000000000000"), -1));
    ASSERT_EQ("bigint2", str_2_BigInt ("+3"), 3);
    SFUintBN bn = 300000000; bn *= 100000000; bn *= 100;
    ASSERT_EQ("bigint2", str_2_BigInt ("+3000000000000000000"), bn);

    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestConverts_2) {
    cout << "Running " << testName << "\n";
    return subTestID==0;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestConverts_3) {
    cout << "Running " << testName << "\n";
    return subTestID==0;
}}

#include "options.h"
//------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        string_q command = nextTokenClear(options.commandList, '\n');
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
toAddr
toAddrOld
toAddress
toBigNum
toBigNum2
toBigNum3
toBloom
toBool
toBool_in
toBoolean
toBytes
toDouble
wei2Ether
toFloat
str_2_Int
str_2_Int32u
str_2_Uint
toTopic
str_2_Uint
toWei
toAscString
toBlock
toHex
toDate
toDayName
toJson
toJson1
toJsonFldList
toLower
toMonthName
toProper
toRecordID
toSpaces
toSpaces2
toString
toTimeStamp
toTrans
toUpper
bnu_2_Hex
to_int
to_long
to_short
to_string
to_uint
to_ulong
to_ushort


fromAddress
fromBloom
fromHash
fromTopic
fromUnsigned
fromWei

asBitmap
asDollars
asEther
asHex
asPct
toString
toStringBN
toStringU

ASSERT_NOT_EQ("c-string non-equality",        foo,  bar      );
ASSERT_EQ    ("string equality",              foo.c_str(), xfoo);
ASSERT_TRUE  ("c-string less than",           (foo < bar)    );
ASSERT_FALSE ("c-string greater than",        (foo > bar)    );
cout << TESTID("base case", 15) << text1 << "\n";
#endif
