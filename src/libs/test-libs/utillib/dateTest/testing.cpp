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
#include "etherlib.h"
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
TEST_F(CThisTest, TestTest_1) {
    cout << "Running " << testName << "\n";

    time_q theDate = time_q(2019, 8, 7, 11, 10, 9);
    ASSERT_EQ("date", theDate, theDate);
    ASSERT_EQ("BOH", BOH(theDate), time_q(2019, 8, 7, 11, 0, 0));
    ASSERT_EQ("BOD", BOD(theDate), time_q(2019, 8, 7, 0, 0, 0));
    ASSERT_EQ("BOW", BOW(theDate), time_q(2019, 8, 4, 0, 0, 0));
    ASSERT_EQ("BOM", BOM(theDate), time_q(2019, 8, 1, 0, 0, 0));
    ASSERT_EQ("BOQ", BOQ(theDate), time_q(2019, 7, 1, 0, 0, 0));
    ASSERT_EQ("BOY", BOY(theDate), time_q(2019, 1, 1, 0, 0, 0));
    ASSERT_EQ("EOH", EOH(theDate), time_q(2019, 8, 7, 11, 59, 59));
    ASSERT_EQ("EOD", EOD(theDate), time_q(2019, 8, 7, 23, 59, 59));
    ASSERT_EQ("EOW", EOW(theDate), time_q(2019, 8, 10, 23, 59, 59));
    ASSERT_EQ("EOM", EOM(theDate), time_q(2019, 8, 31, 23, 59, 59));
    ASSERT_EQ("EOQ", EOQ(theDate), time_q(2019, 9, 30, 23, 59, 59));
    ASSERT_EQ("EOY", EOY(theDate), time_q(2019, 12, 31, 23, 59, 59));

    return true;
}
}

TEST_F(CThisTest, TestTest_2) {
    cout << "Running " << testName << "\n";

    time_q theDate = time_q(2019, 8, 7, 11, 10, 9);
    ASSERT_EQ("earliestDate", earliestDate, time_q(1700, 1, 1, 0, 0, 1));
    ASSERT_EQ("latestDate", latestDate, time_q(2200, 12, 31, 23, 59, 59));
    ASSERT_EQ("AddOneDay", AddOneDay(theDate), time_q(2019, 8, 8, 11, 10, 9));
    ASSERT_EQ("AddOneHour1", AddOneHour(theDate), time_q(2019, 8, 7, 12, 10, 9));
    ASSERT_EQ("AddOneHour2", AddOneHour(EOD(theDate)), time_q(2019, 8, 8, 0, 59, 59));
    ASSERT_EQ("AddOneWeek1", AddOneWeek(theDate), time_q(2019, 8, 14, 11, 10, 9));
    ASSERT_EQ("AddOneWeek2", AddOneWeek(EOM(theDate)), time_q(2019, 9, 7, 23, 59, 59));
    ASSERT_EQ("AddOneMonth1", AddOneMonth(theDate), time_q(2019, 9, 7, 11, 10, 9));
    ASSERT_EQ("AddOneMonth2", AddOneMonth(EOY(theDate)), time_q(2020, 1, 31, 23, 59, 59));
    ASSERT_EQ("AddOneQuarter", AddOneQuarter(theDate), time_q(2019, 11, 7, 11, 10, 9));
    ASSERT_EQ("AddOneYear", AddOneYear(theDate), time_q(2020, 8, 7, 11, 10, 9));
    ASSERT_EQ("SubtractOneDay1", SubtractOneDay(theDate), time_q(2019, 8, 6, 11, 10, 9));
    ASSERT_EQ("SubtractOneDay2", SubtractOneDay(BOM(theDate)), time_q(2019, 7, 31, 0, 0, 0));
    ASSERT_EQ("SubtractOneHour1", SubtractOneHour(theDate), time_q(2019, 8, 7, 10, 10, 9));
    ASSERT_EQ("SubtractOneHour2", SubtractOneHour(BOY(theDate)), time_q(2018, 12, 31, 23, 0, 0));
    ASSERT_EQ("SubtractOneYear", SubtractOneYear(theDate), time_q(2018, 8, 7, 11, 10, 9));
    ASSERT_EQ("DaysInMonth", DaysInMonth(theDate.GetYear(), theDate.GetMonth()), 31);
    ASSERT_EQ("Format(FMT_JSON)", theDate.Format(FMT_JSON), "2019-08-07 11:10:09 UTC");
    ASSERT_EQ("Format(FMT_EXPORT)", theDate.Format(FMT_EXPORT), "2019-08-07T11:10:09");
    ASSERT_EQ("Format(FMT_PARTS)", theDate.Format(FMT_PARTS), "2019|08|07|11|10|09");
    ASSERT_EQ("Format(FMT_SHORT)", theDate.Format(FMT_SHORT), "20190807");

    return true;
}
}

TEST_F(CThisTest, TestTest_3) {
    cout << "Running " << testName << "\n";

    time_q theDate = time_q(2019, 8, 7, 11, 10, 9);
    time_q theDate2 = time_q(2020, 12, 1, 1, 1, 1);
    ASSERT_EQ("earlierOf", earlierOf(theDate, EOY(theDate)), theDate);
    ASSERT_EQ("laterOf", laterOf(theDate, EOY(theDate)), EOY(theDate));
    ASSERT_EQ("getCentury", getCentury(2021), 2000);
    ASSERT_EQ("get2Digit", get2Digit(2021), 21);
    ASSERT_TRUE("isSameYear", isSameYear(theDate, theDate));
    ASSERT_TRUE("isSameQuarter", isSameQuarter(theDate, theDate));
    ASSERT_TRUE("isSameMonth", isSameMonth(theDate, theDate));
    ASSERT_TRUE("isSameWeek", isSameWeek(theDate, theDate));
    ASSERT_TRUE("isSameDay", isSameDay(theDate, theDate));
    ASSERT_TRUE("isSameHour", isSameHour(theDate, theDate));
    ASSERT_FALSE("isSameYear", isSameYear(theDate, theDate2));
    ASSERT_FALSE("isSameQuarter", isSameQuarter(theDate, theDate2));
    ASSERT_FALSE("isSameMonth", isSameMonth(theDate, theDate2));
    ASSERT_FALSE("isSameWeek", isSameWeek(theDate, theDate2));
    ASSERT_FALSE("isSameDay", isSameDay(theDate, theDate2));
    ASSERT_FALSE("isSameHour", isSameHour(theDate, theDate2));
    ASSERT_EQ("MIDDAY", MIDDAY(theDate), time_q(2019, 8, 7, 12, 0, 0));

    return true;
}
}

#define SHOWEM(t)                                                                                                      \
    {                                                                                                                  \
        uint32_t cnt = 0;                                                                                              \
        cout << "\t" << (t) << endl;                                                                                   \
        for (auto d : res)                                                                                             \
            cout << "\t\t" << cnt++ << ": " << d << endl;                                                              \
        res.clear();                                                                                                   \
    }

TEST_F(CThisTest, TestTest_4) {
    cout << "Running " << testName << "\n";

    time_q theDate = time_q(2019, 8, 7, 11, 10, 9);
    ASSERT_EQ("BONH", BONH(theDate), time_q(2019, 8, 7, 12, 0, 0));
    ASSERT_EQ("BOND", BOND(theDate), time_q(2019, 8, 8, 0, 0, 0));
    ASSERT_EQ("BONW", BONW(theDate), time_q(2019, 8, 11, 0, 0, 0));
    ASSERT_EQ("BONM", BONM(theDate), time_q(2019, 9, 1, 0, 0, 0));
    ASSERT_EQ("BONQ", BONQ(theDate), time_q(2019, 10, 1, 0, 0, 0));
    ASSERT_EQ("BONY", BONY(theDate), time_q(2020, 1, 1, 0, 0, 0));

    CTimeArray res;
    expandHourly(res, theDate, EOD(theDate), true);
    SHOWEM("expandHourly");
    expandDaily(res, theDate, EOM(theDate), true);
    SHOWEM("expandDaily");
    expandWeekly(res, BOY(theDate), EOQ(BOY(theDate)), true);
    SHOWEM("expandWeekly");
    expandMonthly(res, BOY(theDate), EOY(theDate), true);
    SHOWEM("expandMonthly");
    expandQuarterly(res, BOY(theDate), EOY(theDate), true);
    SHOWEM("expandQuarterly");
    expandAnnually(res, BOY(theDate), BONY(BONY(EOY(theDate))), true);
    SHOWEM("expandAnnually");

    return true;
}
}

TEST_F(CThisTest, TestTest_5) {
    cout << "Running " << testName << "\n";

    time_q theDate = time_q(2019, 8, 7, 11, 10, 9);

    ASSERT_EQ("BOP(BY_NOTHING)", BOP(BY_NOTHING, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("BOP(BY_YEAR)", BOP(BY_YEAR, theDate), time_q(2019, 1, 1, 0, 0, 0));
    ASSERT_EQ("BOP(BY_QUARTER)", BOP(BY_QUARTER, theDate), time_q(2019, 7, 1, 0, 0, 0));
    ASSERT_EQ("BOP(BY_MONTH)", BOP(BY_MONTH, theDate), time_q(2019, 8, 1, 0, 0, 0));
    ASSERT_EQ("BOP(BY_WEEK)", BOP(BY_WEEK, theDate), time_q(2019, 8, 4, 0, 0, 0));
    ASSERT_EQ("BOP(BY_DAY)", BOP(BY_DAY, theDate), time_q(2019, 8, 7, 0, 0, 0));
    ASSERT_EQ("BOP(BY_HOUR)", BOP(BY_HOUR, theDate), time_q(2019, 8, 7, 11, 0, 0));
    // The following number based routines don't really work, but it's okay since we don't use them
    ASSERT_EQ("BOP(BY_1)", BOP(BY_1, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("BOP(BY_10)", BOP(BY_10, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("BOP(BY_100)", BOP(BY_100, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("BOP(BY_1000)", BOP(BY_1000, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("BOP(BY_10000)", BOP(BY_10000, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("BOP(BY_100000)", BOP(BY_100000, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("BOP(BY_1000000)", BOP(BY_1000000, theDate), time_q(2019, 8, 7, 11, 10, 9));

    ASSERT_EQ("EOP(BY_NOTHING)", EOP(BY_NOTHING, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("EOP(BY_YEAR)", EOP(BY_YEAR, theDate), time_q(2019, 12, 31, 23, 59, 59));
    ASSERT_EQ("EOP(BY_QUARTER)", EOP(BY_QUARTER, theDate), time_q(2019, 9, 30, 23, 59, 59));
    ASSERT_EQ("EOP(BY_MONTH)", EOP(BY_MONTH, theDate), time_q(2019, 8, 31, 23, 59, 59));
    ASSERT_EQ("EOP(BY_WEEK)", EOP(BY_WEEK, theDate), time_q(2019, 8, 10, 23, 59, 59));
    ASSERT_EQ("EOP(BY_DAY)", EOP(BY_DAY, theDate), time_q(2019, 8, 7, 23, 59, 59));
    ASSERT_EQ("EOP(BY_HOUR)", EOP(BY_HOUR, theDate), time_q(2019, 8, 7, 11, 59, 59));
    // The following number based routines don't really work, but it's okay since we don't use them
    ASSERT_EQ("EOP(BY_1)", EOP(BY_1, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("EOP(BY_10)", EOP(BY_10, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("EOP(BY_100)", EOP(BY_100, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("EOP(BY_1000)", EOP(BY_1000, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("EOP(BY_10000)", EOP(BY_10000, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("EOP(BY_100000)", EOP(BY_100000, theDate), time_q(2019, 8, 7, 11, 10, 9));
    ASSERT_EQ("EOP(BY_1000000)", EOP(BY_1000000, theDate), time_q(2019, 8, 7, 11, 10, 9));

    ASSERT_EQ("per_2_Str(BY_NOTHING)", per_2_Str(BY_NOTHING), "[{MONTH}],[{DAY}]");
    ASSERT_EQ("per_2_Str(BY_YEAR)", per_2_Str(BY_YEAR), "[{YEAR}]");
    ASSERT_EQ("per_2_Str(BY_QUARTER)", per_2_Str(BY_QUARTER), "[{QUARTER}]");
    ASSERT_EQ("per_2_Str(BY_MONTH)", per_2_Str(BY_MONTH), "[{MONTH}]");
    ASSERT_EQ("per_2_Str(BY_WEEK)", per_2_Str(BY_WEEK), "[{WEEK}]");
    ASSERT_EQ("per_2_Str(BY_DAY)", per_2_Str(BY_DAY), "[{DAY}]");
    ASSERT_EQ("per_2_Str(BY_HOUR)", per_2_Str(BY_HOUR), "[{HOUR}]");
    // The following number based routines don't really work, but it's okay since we don't use them
    ASSERT_EQ("per_2_Str(BY_1)", per_2_Str(BY_1), "");
    ASSERT_EQ("per_2_Str(BY_10)", per_2_Str(BY_10), "");
    ASSERT_EQ("per_2_Str(BY_100)", per_2_Str(BY_100), "");
    ASSERT_EQ("per_2_Str(BY_1000)", per_2_Str(BY_1000), "");
    ASSERT_EQ("per_2_Str(BY_10000)", per_2_Str(BY_10000), "");
    ASSERT_EQ("per_2_Str(BY_100000)", per_2_Str(BY_100000), "");
    ASSERT_EQ("per_2_Str(BY_1000000)", per_2_Str(BY_1000000), "");

    // extern bool isSamePeriod(period_t period, const time_q& t1, const time_q& t2);
    // extern bool isSamePeriod(period_t period, blknum_t t1, blknum_t t2);

    return true;
}
}

//------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    LOAD_TEST(TestTest_1);
    LOAD_TEST(TestTest_2);
    LOAD_TEST(TestTest_3);
    LOAD_TEST(TestTest_4);
    LOAD_TEST(TestTest_5);
    return testing::RUN_ALL_TESTS();
}
