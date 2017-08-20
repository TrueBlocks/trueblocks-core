#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

namespace qblocks {
    //---------------------------------------------------------------------------
    inline bool isTestMode(void) {
        return (SFString(getenv("TEST_MODE")) == "true");
    }
}  // namespace qblocks

namespace testing {

    typedef bool (*PF)(int testID);
    class Test {
    public:
        static PF funcs[100];
        static int nFuncs;
        Test(void)  { for (int i=0;i<100;i++) { funcs[i] = NULL; } }
        virtual void SetUp(void) = 0;
        virtual void TearDown(void) = 0;
        static void addFunc(PF func) {
            for (int i=0;i<nFuncs;i++) {
                if (funcs[i])
                    return;
            }
            funcs[nFuncs++] = func;
        }
    };

#define LOAD_TEST(funcName) \
testing::Test::addFunc(func_##funcName);

#define TEST_F(testClass, funcName) \
extern bool func_##funcName(int testID); \
bool func_##funcName(int testID) { \
int subTestID = 0; \
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
    for (int i = 0 ; i < testing::Test::nFuncs ; i++)
        if (testing::Test::funcs[i]) {
            cerr << i << ". ";
            result |= !((*(testing::Test::funcs[i]))(i));
        }
    return result;
}
