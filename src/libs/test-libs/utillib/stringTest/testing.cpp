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

// Changing this between string_q and SFString helps migrating away from quickBlocks code
#define TEST_STR string_q

//------------------------------------------------------------------------
class CThisTest : public testing::Test {
public:
                 CThisTest(void) : Test() {}
    virtual void SetUp    (void) {}
    virtual void TearDown (void) {}
};

//------------------------------------------------------------------------
TEST_F(CThisTest, TestRelational) {
    string foo = "alpha"; TEST_STR xfoo(foo.c_str());
    string bar = "beta";  TEST_STR xbar(bar.c_str());

    cerr << "Running " << testName << "\n";
    cerr << "\tc-string  -- foo:  " << foo  << " bar: " << bar << "\n";
    cerr << "\tqb-string -- xfoo: " << xfoo << " xbar: " << xbar << "\n\n";

    ASSERT_NOT_EQ("c-string non-equality",        foo,  bar      );
    ASSERT_NOT_EQ("qb-string non-equality",       xfoo, xbar     );
    ASSERT_EQ    ("string equality",              foo.c_str(), xfoo);
    ASSERT_TRUE  ("c-string less than",           (foo < bar)    );
    ASSERT_TRUE  ("qb-string less than",          (xfoo < xbar)  );
    ASSERT_FALSE ("c-string greater than",        (foo > bar)    );
    ASSERT_FALSE ("qb-string greater than",       (xfoo > xbar)  );
    ASSERT_TRUE  ("c-string less than or eq",     (foo <= bar)   );
    ASSERT_TRUE  ("qb-string less than or eq",    (xfoo <= xbar) );
    ASSERT_FALSE ("c-string greater than or eq",  (foo >= bar)   );
    ASSERT_FALSE ("qb-string greater than or eq", (xfoo >= xbar) );
    ASSERT_FALSE ("c-string min",                 (min(foo, bar) == bar) );
    ASSERT_TRUE  ("c-string max",                 (max(foo, bar) == bar) );
    ASSERT_FALSE ("qb-string min",                (min(xfoo, xbar) == xbar) );
    ASSERT_TRUE  ("qb-string max",                (max(xfoo, xbar) == xbar) );

    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestCompare) {
    string str1("green apple"); TEST_STR xstr1("green apple");
    string str2("red apple");   TEST_STR xstr2("red apple");

    cerr << "Running " << testName << "\n";
    cerr << "\tc-string  -- str1:  " <<  str1  << " str2: "  << str2  << "\n";
    cerr << "\tqb-string -- xstr1: " << xstr1 << " xstr2: " << xstr2 << "\n\n";

    ASSERT_NOT_EQ("c-string compare",             str1.compare( str2 ), 0);
    ASSERT_NOT_EQ("qb-string compare",           xstr1.compare( xstr2), 0);
    ASSERT_NOT_EQ("c-string compare",             str1.compare( str2 ), 0);
    ASSERT_NOT_EQ("qb-string compare",           xstr1.compare( xstr2), 0);
    ASSERT_EQ    ("c-string partial compare",     str1.compare( 6, 5,"apple"),  0);
    ASSERT_EQ    ("qb-string partial compare",   xstr1.compare( 6, 5,"apple"),  0);
    ASSERT_EQ    ("c-string partial compare 2",   str2.compare( str2.size()-5,  5, "apple"), 0);
    ASSERT_EQ    ("qb-string partial compare 2", xstr2.compare( str2.size()-5,  5, "apple"), 0);
    ASSERT_EQ    ("c-string double partial",      str1.compare( 6, 5,  str2, 4, 5), 0);
    ASSERT_EQ    ("qb-string double partial",    xstr1.compare( 6, 5, xstr2, 4, 5), 0);

    return true;
}}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestAssignment) {
    string str1, str2, str3;
    TEST_STR xstr1, xstr2, xstr3;

     str1 = "Test string: ";  str2 = 'x';
    xstr1 = "Test string: "; xstr2 = TEST_STR('x');

     str3 =  str1 +  str2;
    xstr3 = xstr1 + xstr2;

    ASSERT_TRUE("c-string set1",   str1 == "Test string: ");
    ASSERT_TRUE("qb-string set1", xstr1 == "Test string: ");
    ASSERT_TRUE("c-string set2",   str2 == "x");
    ASSERT_TRUE("qb-string set2", xstr2 == "x");
    ASSERT_TRUE("c-string set3",   str3 == "Test string: x");
    ASSERT_TRUE("qb-string set3", xstr3 == "Test string: x");
    ASSERT_TRUE("set1 equal",      str1 == xstr1.str());
    ASSERT_TRUE("set2 equal",      str2 == xstr2.str());
    ASSERT_TRUE("set3 equal",      str3 == xstr3.str());

    return true;
}}

//------------------------------------------------------------------------
void testCStr(void) {

    char str[] = "Please split this sentence into tokens";
    char *token;
    char *rest = str;
    while ((token = strtok_r(rest, " ", &rest)))
        cout << "\t" << padRight(token,8,' ') << "\t" << (rest ? rest : "") << "\n";
    cout << "\n";
    char str1[] = "Please|split|that|sentence|into|tokens";
    char *token1;
    char *rest1 = str1;
    while ((token1 = strtok_r(rest1, "|", &rest1)))
        cout << "\t" << padRight(token1,8,' ') << "\t" << (rest1 ? rest1 : "") << "\n";
}

#include "options.h"
//------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        options.testNum = 3;
        switch (options.testNum) {
            case 0: LOAD_TEST(TestRelational); break;
            case 1: LOAD_TEST(TestCompare);    break;
            case 2:           testCStr();      break;
            case 3: LOAD_TEST(TestAssignment); break;
        }
    }

    return RUN_ALL_TESTS();
}
