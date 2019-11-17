#pragma once
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

namespace testing {  // NOLINT

typedef bool (*PF)(uint64_t testID);
class Test {
  public:
    // TODO(tjayrush): global data
    static PF funcs[100];
    static uint64_t nFuncs;
    Test(void) {
        memset(funcs, '\0', sizeof(funcs));
    }
    virtual ~Test(void) {
    }
    virtual void SetUp(void) = 0;
    virtual void TearDown(void) = 0;
    static void addFunc(PF func) {
        for (size_t i = 0; i < nFuncs; i++) {
            if (funcs[i])
                return;
        }
        funcs[nFuncs++] = func;
    }
};

#define LOAD_TEST(funcName) testing::Test::addFunc(func_##funcName);

#define TEST_F(testClass, funcName)                                                                                    \
    extern bool func_##funcName(uint64_t testID);                                                                      \
    bool func_##funcName(uint64_t testID) {                                                                            \
        uint64_t subTestID = 0;                                                                                        \
        string_q testName = #funcName;

#define TESTID(msg, wid) "\t" << padNum3(testID) << "." << padNum3(subTestID++) << " " << padRight(msg, wid) << " ==> "

#define TEST_ID() TESTID("", 1)

#define ASSERT_TRUE(msg, test)                                                                                         \
    {                                                                                                                  \
        cout << TESTID(msg, 32);                                                                                       \
        if ((test)) {                                                                                                  \
            cout << "passed '" << #test << "' is true\n";                                                              \
        } else {                                                                                                       \
            cout << "failed '" << #test << "' is false\n";                                                             \
            return false;                                                                                              \
        }                                                                                                              \
    }

#define ASSERT_FALSE(msg, test)                                                                                        \
    {                                                                                                                  \
        cout << TESTID(msg, 32);                                                                                       \
        if (!(test)) {                                                                                                 \
            cout << "passed '" << #test << "' is false\n";                                                             \
        } else {                                                                                                       \
            cout << "failed '" << #test << "' is true\n";                                                              \
            return false;                                                                                              \
        }                                                                                                              \
    }

#define ASSERT_NOT_EQ(msg, a, b)                                                                                       \
    {                                                                                                                  \
        cout << TESTID(msg, 32);                                                                                       \
        if ((a) != (b)) {                                                                                              \
            cout << "passed '" << #a << "' is not equal to '" << #b << "'"                                             \
                 << "\n";                                                                                              \
        } else {                                                                                                       \
            cout << "failed '" << #a << "' should not be equal to '" << #b << "'"                                      \
                 << "\n";                                                                                              \
            return false;                                                                                              \
        }                                                                                                              \
    }

#define ASSERT_EQ(msg, a, b)                                                                                           \
    {                                                                                                                  \
        cout << TESTID(msg, 32);                                                                                       \
        if ((a) == (b)) {                                                                                              \
            cout << "passed '" << #a << "' is equal to '" << #b << "'"                                                 \
                 << "\n";                                                                                              \
        } else {                                                                                                       \
            cout << "failed '" << #a << "' should be equal to '" << #b << "' but got '" << a << "'\n";                 \
            return false;                                                                                              \
        }                                                                                                              \
    }

#define SHOULD_THROW(msg, a)                                                                                           \
    cout << TESTID(msg, 32) << "The test should throw. ";                                                              \
    try {                                                                                                              \
        (a);                                                                                                           \
        cout << " --> Test did not throw\n";                                                                           \
    } catch (exception & e) {                                                                                          \
        cout << " --> Test threw with message: " << string_q(e.what()).substr(0, 12) << "\n";                          \
    }

#define SHOULD_NOT_THROW(msg, a)                                                                                       \
    cout << TESTID(msg, 32) << "The test should not throw. ";                                                          \
    try {                                                                                                              \
        (a);                                                                                                           \
        cout << " --> Test did not throw\n";                                                                           \
    } catch (exception & e) {                                                                                          \
        cout << " --> Test threw with message: " << string_q(e.what()).substr(0, 12) << "\n";                          \
    }

inline int RUN_ALL_TESTS(void) {
    bool result = false;
    for (size_t i = 0; i < testing::Test::nFuncs; i++)
        if (testing::Test::funcs[i]) {
            cerr << i << ". ";
            result |= !((*(testing::Test::funcs[i]))(i));
        }
    return result;
}

}  // namespace testing
