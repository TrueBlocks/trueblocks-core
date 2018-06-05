#pragma once
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

namespace qblocks {
    //---------------------------------------------------------------------------
    inline bool isTestMode(void) {
        return (SFString(getenv("TEST_MODE")) == "true");
    }
}  // namespace qblocks

namespace testing {

    typedef bool (*PF)(uint64_t testID);
    class Test {
    public:
        static PF funcs[100];
        static uint64_t nFuncs;
        Test(void)  { for (uint32_t i=0;i<100;i++) { funcs[i] = NULL; } }
        virtual ~Test(void) {}
        virtual void SetUp(void) = 0;
        virtual void TearDown(void) = 0;
        static void addFunc(PF func) {
            for (uint32_t i=0;i<nFuncs;i++) {
                if (funcs[i])
                    return;
            }
            funcs[nFuncs++] = func;
        }
    };

#define LOAD_TEST(funcName) \
testing::Test::addFunc(func_##funcName);

#define TEST_F(testClass, funcName) \
extern bool func_##funcName(uint64_t testID); \
bool func_##funcName(uint64_t testID) { \
uint64_t subTestID = 0; \
SFString testName = #funcName; \

#define ASSERT_TRUE(msg, test) { \
cerr << "\t" << padNum3(testID) << "." << padNum3(subTestID++) << ". " << padRight(msg, 32) << " ==> "; \
if ((test)) { \
cerr << "passed '" << #test<< "' is true\n"; \
} else { \
cerr << "failed '" << #test << "' is false\n"; \
return false; \
} \
}

#define ASSERT_FALSE(msg, test) { \
cerr << "\t" << padNum3(testID) << "." << padNum3(subTestID++) << ". " << padRight(msg, 32) << " ==> "; \
if (!(test)) { \
cerr << "passed '" << #test << "' is false\n"; \
} else { \
cerr << "failed '" << #test << "' is true\n"; \
return false; \
} \
}

#define ASSERT_NOT_EQ(msg, a, b) { \
cerr << "\t" << padNum3(testID) << "." << padNum3(subTestID++) << ". " << padRight(msg, 32) << " ==> "; \
if ((a) != (b)) { \
cerr << "passed '" << #a << "' is not equal to '" << #b << "'" << "\n"; \
} else { \
cerr << "failed '" << #a << "' should not be equal to '" << #b << "'" << "\n"; \
return false; \
} \
}

#define ASSERT_EQ(msg, a, b) { \
cerr << "\t" << padNum3(testID) << "." << padNum3(subTestID++) << ". " << padRight(msg, 32) << " ==> "; \
if ((a) == (b)) { \
cerr << "passed '" << #a << "' is equal to '" << #b << "'" << "\n"; \
} else { \
cerr << "failed '" << #a << "' should be equal to '" << #b << "'" << "\n"; \
return false; \
} \
}
}  // namespace testing

inline int RUN_ALL_TESTS(void) {
    bool result = false;
    for (uint32_t i = 0 ; i < testing::Test::nFuncs ; i++)
        if (testing::Test::funcs[i]) {
            cerr << i << ". ";
            result |= !((*(testing::Test::funcs[i]))(i));
        }
    return result;
}
