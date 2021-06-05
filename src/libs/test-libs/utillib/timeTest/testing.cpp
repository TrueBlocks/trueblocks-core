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
TEST_F(CThisTest, TestTime) {
    time_q time1(2016, 10, 1, 12, 0, 0);
    time_q time2(2018, 6, 2, 12, 0, 0);

    cerr << "Running " << testName << "\n";
    cerr << "\ttime1:  " << time1 << "\n";
    cerr << "\ttime2:  " << time2 << "\n";

    ASSERT_FALSE("time1 > time2", (time1 > time2));
    ASSERT_TRUE("time1 !< time2", (time1 < time2));
    ASSERT_TRUE("time1.month == 10", (time1.GetMonth() == 10));
    ASSERT_TRUE("time2.day == 2", (time2.GetDay() == 2));

    return true;
}
}

namespace qblocks {}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestAdvance) {
    time_q time1(2018, 6, 2, 12, 22, 12);
    time_q time2(2018, 12, 31, 23, 22, 12);

    cerr << "Running " << testName << endl;
    cerr << "\ttime1:  " << time1 << endl;
    cerr << "\ttime2:  " << time2 << endl;

    ASSERT_TRUE("time1.Format()", (time1.Format(FMT_EXPORT) == "2018-06-02T12:22:12"));
    ASSERT_TRUE("BOH(time1)", (BOH(time1).Format(FMT_EXPORT) == "2018-06-02T12:00:00"));
    ASSERT_TRUE("BONH(time1)", (BONH(time1).Format(FMT_EXPORT) == "2018-06-02T13:00:00"));
    ASSERT_TRUE("BOND(time1)", (BOND(time1).Format(FMT_EXPORT) == "2018-06-03T00:00:00"));
    ASSERT_TRUE("BOND(time2)", (BOND(time2).Format(FMT_EXPORT) == "2019-01-01T00:00:00"));
    ASSERT_TRUE("BONW(time1)", (BONW(time1).Format(FMT_EXPORT) == "2018-06-03T00:00:00"));
    ASSERT_TRUE("BONW(BONW(time1))", (BONW(BONW(time1)).Format(FMT_EXPORT) == "2018-06-10T00:00:00"));
    ASSERT_TRUE("BONM(time1)", (BONM(time1).Format(FMT_EXPORT) == "2018-07-01T00:00:00"));
    ASSERT_TRUE("BONM(BONM(time1))", (BONM(BONM(time1)).Format(FMT_EXPORT) == "2018-08-01T00:00:00"));
    ASSERT_TRUE("BONM(BONM(time2))", (BONM(BONM(time2)).Format(FMT_EXPORT) == "2019-02-01T00:00:00"));
    ASSERT_TRUE("BONY(time1)", (BONY(time1).Format(FMT_EXPORT) == "2019-01-01T00:00:00"));
    ASSERT_TRUE("BONY(BONH(time2))", (BONY(BONH(time2)).Format(FMT_EXPORT) == "2020-01-01T00:00:00"));
    ASSERT_TRUE("BONY(BONY(time2))", (BONY(BONY(time2)).Format(FMT_EXPORT) == "2020-01-01T00:00:00"));
    ASSERT_TRUE("BONQ(time1)", (BONQ(time1).Format(FMT_EXPORT) == "2018-07-01T00:00:00"));

    return true;
}
}

//------------------------------------------------------------------------
TEST_F(CThisTest, TestAdvance2) {
    time_q time1(2018, 6, 2, 12, 22, 12);
    time_q time2(2018, 12, 31, 23, 22, 12);

    cerr << "Running " << testName << endl;
    cerr << "\ttime1:  " << time1 << endl;
    cerr << "\ttime2:  " << time2 << endl;
    ASSERT_TRUE("time1", (time1.Format(FMT_EXPORT) == "2018-06-02T12:22:12"))

    CTimeArray ta;

    cout << endl << "20 hourly expansions from " << time1.Format(FMT_EXPORT) << endl;
    expandHourly(ta, time1, time_q(2018, 6, 3, 8, 0, 0));
    for (auto t : ta)
        cout << t << endl;
    ta.clear();

    cout << endl << string_q(120, '-') << endl << "20 daily expansions from " << time1.Format(FMT_EXPORT) << endl;
    expandDaily(ta, time1, time_q(2018, 6, 14, 0, 0, 0));
    for (auto t : ta)
        cout << t << endl;
    ta.clear();

    cout << endl
         << string_q(120, '-') << endl
         << "20 weekly expansions no fallback from " << time1.Format(FMT_EXPORT) << endl;
    expandWeekly(ta, time1, time_q(2018, 10, 14, 0, 0, 0), false);
    for (auto t : ta)
        cout << t << endl;
    ta.clear();

    cout << endl << string_q(120, '-') << endl << "20 monthly expansions from " << time1.Format(FMT_EXPORT) << endl;
    expandMonthly(ta, time1, time_q(2020, 2, 1, 0, 0, 0));
    for (auto t : ta)
        cout << t << endl;
    ta.clear();

    cout << endl << string_q(120, '-') << endl << "20 quarterly expansions from " << time1.Format(FMT_EXPORT) << endl;
    expandQuarterly(ta, time1, time_q(2023, 04, 01, 0, 0, 0));
    for (auto t : ta)
        cout << t << endl;
    ta.clear();

    cout << endl << string_q(120, '-') << endl << "20 annual expansions from " << time1.Format(FMT_EXPORT) << endl;
    expandAnnually(ta, time1, time_q(2038, 1, 1, 0, 0, 0));
    for (auto t : ta)
        cout << t << endl;
    ta.clear();

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

        switch (options.testNum) {
            case 0:
                LOAD_TEST(TestTime);
                break;
            case 1:
                LOAD_TEST(TestAdvance);
                break;
            case 2:
                LOAD_TEST(TestAdvance2);
                break;
        }
    }

    return testing::RUN_ALL_TESTS();
}
