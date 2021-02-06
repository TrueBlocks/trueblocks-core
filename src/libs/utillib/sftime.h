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
#include "sfos.h"

namespace qblocks {

using timestamp_t = int64_t;

class time_q;
enum MONTH { JANUARY = 1, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER };

enum DayOfWeek { SUNDAY = 1, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY };

struct CDateStruct {
    uint32_t m_Year;
    uint32_t m_Month;
    uint32_t m_Day;
    uint32_t m_Hour;
    uint32_t m_Minute;
    uint32_t m_Second;
};

//-------------------------------------------------------------------------
// A Date class with a granularity of 1 second
//-------------------------------------------------------------------------
class time_q {
  private:
    class CDate {
        // Count of days since 15 October 1582 (start of Gregorian Calendar)
        uint64_t m_nDays;

        CDate(void);
        CDate(const CDate& dt);
        CDate(uint32_t year, uint32_t month, uint32_t day);
        CDate(uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek);
        CDate(const string_q& dateStr, const string_q& fmtStr);

        explicit CDate(int64_t days);
        explicit CDate(const tm& sysTime);

        CDate& operator=(const CDate& date);

        uint32_t GetYear(void) const;
        uint32_t GetMonth(void) const;
        uint32_t GetDay(void) const;

        int64_t GetTotalDays(void) const;

        CDate operator+(int32_t days) const;
        CDate operator-(int32_t days) const;

        CDate& operator+=(int32_t days);
        CDate& operator-=(int32_t days);

        CDate& setValues(uint32_t y, uint32_t m, uint32_t d);
        CDateStruct getDateStruct(void) const;

        friend class time_q;
        friend uint32_t getWeekOfMonth(const CDate& date);
        friend uint32_t getDayOfWeek(const CDate& date);
    };

    //----------------------------------------------------------------------------
    // Used by time_q to hold the time portion of the date
    //----------------------------------------------------------------------------
    class CTimeOfDay {
      public:
        CTimeOfDay(void);
        CTimeOfDay(const CTimeOfDay& tod);
        CTimeOfDay(uint32_t h, uint32_t m, uint32_t s);
        CTimeOfDay(const string_q& dateStr, const string_q& fmtStr);

        explicit CTimeOfDay(const tm& sysTime);
        explicit CTimeOfDay(uint32_t secs);

        uint32_t GetHour(void) const;
        uint32_t GetMinute(void) const;
        uint32_t GetSecond(void) const;
        uint32_t GetTotalSeconds(void) const;

        uint32_t m_nSeconds;
    };

  public:
    time_q(void);
    time_q(const time_q& date);

    time_q(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t sec);
    time_q(uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek, uint32_t hour, uint32_t minute,
           uint32_t sec);
    time_q(uint32_t days, uint32_t hour, uint32_t minute, uint32_t sec);
    time_q(const CDate& date, const CTimeOfDay& tod);
    time_q(const string_q& dateStr, const string_q& fmtStr);

    explicit time_q(const tm& sysTime, bool useDayOfWeek = false);

    time_q& operator=(const time_q& date);

    bool IsValid(void) const;

    time_q operator+(const uint32_t& ts) const;
    time_q operator-(const uint32_t& ts) const;
    int64_t operator-(time_q& date);

    bool operator==(const time_q& date) const;
    bool operator!=(const time_q& date) const;

    bool operator>(const time_q& date) const;
    bool operator<(const time_q& date) const;

    bool operator>=(const time_q& date) const;
    bool operator<=(const time_q& date) const;

    uint32_t GetDay(void) const;
    uint32_t GetMonth(void) const;
    uint32_t GetYear(void) const;
    uint32_t GetHour(void) const;
    uint32_t GetMinute(void) const;
    uint32_t GetSecond(void) const;
    uint32_t GetDayOfWeek(void) const;
    timestamp_t GetTotalSeconds(void) const;

    bool onTheHour(void) const;

    string_q Format(const string_q& fmt) const;

    CDate getDatePart(void) const;
    CTimeOfDay getTimePart(void) const;

    // Count of seconds from same epoch as CDate uses
    int64_t m_nSeconds;

    friend uint32_t getDayOfWeek(const CDate& date);
};
typedef vector<time_q> CTimeArray;

//-----------------------------------------------------------------------------------
extern time_q Now(void);

//---------------------------------------------------------------------------------------------
extern ostream& operator<<(ostream& os, const time_q& x);

//---------------------------------------------------------------------------------------------
extern const time_q latestDate;
extern const time_q earliestDate;

#define FMT_JSON string_q("%Y-%m-%d %H:%M:%S UTC")
#define FMT_EXPORT string_q("%Y-%m-%dT%H:%M:%S")
#define FMT_PARTS string_q("%Y|%m|%d|%H|%M|%S")
#define FMT_SHORT string_q("%Y%m%d")

//---------------------------------------------------------------------------------------------
inline time_q BOH(const time_q& date) {
    // H:00:00
    return time_q(date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), 0, 0);
}

//---------------------------------------------------------------------------------------------
inline time_q EOH(const time_q& date) {
    // H:59:59
    return time_q(date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), 59, 59);
}

//---------------------------------------------------------------------------------------------
inline time_q BOD(const time_q& date) {
    // 12:00:00 am
    return time_q(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
}

//---------------------------------------------------------------------------------------------
inline time_q EOD(const time_q& date) {
    // 11:59:59 pm
    return time_q(date.GetYear(), date.GetMonth(), date.GetDay(), 23, 59, 59);
}

//---------------------------------------------------------------------------------------------
extern time_q BOW(const time_q& tm);
extern time_q EOW(const time_q& tm);

//---------------------------------------------------------------------------------------------
inline time_q BOM(const time_q& date) {
    return time_q(date.GetYear(), date.GetMonth(), 1, 0, 0, 0);
}
extern time_q EOM(const time_q& tm);

//---------------------------------------------------------------------------------------------
inline time_q BOQ(const time_q& date) {
    return time_q(date.GetYear(), uint32_t((date.GetMonth() - 1) / 3) * 3 + 1, 1, 0, 0, 0);
}
extern time_q EOQ(const time_q& tm);

//---------------------------------------------------------------------------------------------
inline time_q BOY(const time_q& date) {
    return time_q(date.GetYear(), 1, 1, 0, 0, 0);
}
extern time_q EOM(const time_q& tm);

//---------------------------------------------------------------------------------------------
inline time_q MIDDAY(const time_q& date) {
    // 12:00 noon
    return time_q(date.GetYear(), date.GetMonth(), date.GetDay(), 12, 0, 0);
}

//---------------------------------------------------------------------------------------------
inline time_q earlierOf(const time_q& one, const time_q& two) {
    if (one < two)
        return one;
    else if (two < one)
        return two;
    return one;
}

//---------------------------------------------------------------------------------------------
inline time_q laterOf(const time_q& one, const time_q& two) {
    if (one > two)
        return one;
    else if (two > one)
        return two;
    return one;
}

//---------------------------------------------------------------------------------------------
inline uint32_t getCentury(uint32_t year) {
    return ((year / 100) * 100);
}

//---------------------------------------------------------------------------------------------
inline uint32_t get2Digit(uint32_t year) {
    return year - getCentury(year);
}

//---------------------------------------------------------------------------------------------
extern uint32_t DaysInMonth(uint32_t year, uint32_t month);
extern time_q AddOneDay(const time_q& date);
extern time_q SubtractOneDay(const time_q& date);
extern time_q AddOneHour(const time_q& date);
extern time_q AddOneWeek(const time_q& date);
extern time_q AddOneMonth(const time_q& date);
extern time_q AddOneQuarter(const time_q& date);
extern time_q AddOneYear(const time_q& date);

//---------------------------------------------------------------------------------------------
inline time_q BONH(const time_q& date) {
    return BOH(earlierOf(latestDate, AddOneHour(date)));
}

//---------------------------------------------------------------------------------------------
inline time_q BOND(const time_q& date) {
    return BOD(earlierOf(latestDate, AddOneDay(date)));
}

//---------------------------------------------------------------------------------------------
inline time_q BONW(const time_q& date) {
    return BOW(earlierOf(latestDate, AddOneWeek(date)));
}

//---------------------------------------------------------------------------------------------
inline time_q BONM(const time_q& date) {
    return BOM(earlierOf(latestDate, AddOneMonth(date)));
}

//---------------------------------------------------------------------------------------------
inline time_q BONQ(const time_q& date) {
    return BOQ(earlierOf(latestDate, AddOneQuarter(date)));
}

//---------------------------------------------------------------------------------------------
inline time_q BONY(const time_q& date) {
    return BOY(earlierOf(latestDate, AddOneYear(date)));
}

//------------------------------------------------------------------
extern time_q fileLastModifyDate(const string_q& filename);

//------------------------------------------------------------------------
extern bool expandHourly(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback = true);
extern bool expandDaily(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback = true);
extern bool expandWeekly(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback = true);
extern bool expandMonthly(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback = true);
extern bool expandQuarterly(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback = true);
extern bool expandAnnually(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback = true);

//------------------------------------------------------------------------
typedef struct {
    time_q fileTime;
    string_q fileName;
} fileInfo;
extern bool getNewestFile(const string_q& path, void* data);
extern fileInfo getNewestFileInFolder(const string_q& path);

using blknum_t = uint64_t;

typedef enum {
    BY_NOTHING = 0,
    BY_YEAR = 20,  // opaque value anyway, to avoid conflict with below
    BY_MONTH,
    BY_WEEK,
    BY_DAY,
    BY_HOUR,
    BY_1 = 1,  // non-opaque values
    BY_10 = 10,
    BY_100 = 100,
    BY_1000 = 1000,
    BY_10000 = 10000,
    BY_100000 = 100000,
    BY_1000000 = 1000000
} period_t;
extern bool isSameYear(const time_q& t1, const time_q& t2);
extern bool isSameMonth(const time_q& t1, const time_q& t2);
extern bool isSameWeek(const time_q& t1, const time_q& t2);
extern bool isSameDay(const time_q& t1, const time_q& t2);
extern bool isSameHour(const time_q& t1, const time_q& t2);
extern bool isSamePeriod(period_t period, const time_q& t1, const time_q& t2);
extern bool isSamePeriod(period_t period, blknum_t t1, blknum_t t2);

extern string_q per_2_Str(period_t period);

}  // namespace qblocks
