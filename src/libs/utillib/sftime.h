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
#include "sfos.h"
#include "conversions.h"

namespace qblocks {

    using timestamp_t = int64_t;

    class time_q;
    enum MONTH {
        JANUARY = 1, FEBRUARY, MARCH,
        APRIL, MAY, JUNE,
        JULY, AUGUST, SEPTEMBER,
        OCTOBER, NOVEMBER, DECEMBER
    };

    enum DayOfWeek {
        SUNDAY = 1, MONDAY, TUESDAY,
        WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
    };

    struct SFDateStruct {
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
        class SFDate {
            // Count of days since 15 October 1582 (start of Gregorian Calendar)
            uint64_t m_nDays;

            SFDate(void);
            SFDate(const SFDate& dt);
            SFDate(uint32_t year, uint32_t month, uint32_t day);
            SFDate(uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek);
            SFDate(const string_q& dateStr, const string_q& fmtStr);

            explicit SFDate(int64_t days);
            explicit SFDate(const tm& sysTime);

            SFDate& operator=(const SFDate& date);

            uint32_t GetYear(void) const;
            uint32_t GetMonth(void) const;
            uint32_t GetDay(void) const;

            int64_t GetTotalDays(void) const;

            SFDate operator+(int32_t days) const;
            SFDate operator-(int32_t days) const;

            SFDate& operator+=(int32_t days);
            SFDate& operator-=(int32_t days);

            SFDate& setValues(uint32_t y, uint32_t m, uint32_t d);
            SFDateStruct getDateStruct(void) const;

            friend class time_q;
            friend uint32_t getWeekOfMonth(const SFDate& date);
            friend uint32_t getDayOfWeek(const SFDate& date);
        };

        //----------------------------------------------------------------------------
        // Used by time_q to hold the time portion of the date
        //----------------------------------------------------------------------------
        class SFTimeOfDay {
public:
            SFTimeOfDay(void);
            SFTimeOfDay(const SFTimeOfDay& tod);
            SFTimeOfDay(uint32_t h, uint32_t m, uint32_t s);
            SFTimeOfDay(const string_q& dateStr, const string_q& fmtStr);

            explicit SFTimeOfDay(const tm& sysTime);
            explicit SFTimeOfDay(uint32_t secs);

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
        time_q(uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek,
                            uint32_t hour, uint32_t minute, uint32_t sec);
        time_q(uint32_t days, uint32_t hour, uint32_t minute, uint32_t sec);
        time_q(const SFDate& date, const SFTimeOfDay& tod);
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

        SFDate      getDatePart(void) const;
        SFTimeOfDay getTimePart(void) const;

        // Count of seconds from same epoch as SFDate uses
        int64_t m_nSeconds;

        friend uint32_t getDayOfWeek(const SFDate& date);
    };

    //-----------------------------------------------------------------------------------
    extern time_q Now(void);

    //---------------------------------------------------------------------------------------------
    extern ostream &operator <<(ostream &os, const time_q& x);

    //---------------------------------------------------------------------------------------------
    extern const time_q latestDate;
    extern const time_q earliestDate;

#define FMT_JSON string_q("%Y-%m-%d %H:%M:%S UTC")

    //---------------------------------------------------------------------------------------------
    extern uint32_t DaysInMonth(uint32_t year, uint32_t month);
    extern time_q   AddOneDay(const time_q& date);
    extern time_q   SubtractOneDay(const time_q& date);

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
    inline time_q MIDDAY(const time_q& date) {
        // 12:00 noon
        return time_q(date.GetYear(), date.GetMonth(), date.GetDay(), 12, 0, 0);
    }

    //---------------------------------------------------------------------------------------------
    inline bool isInRange(const time_q& ref, const time_q& start, const time_q& end) {
        return (start <= ref && end >= ref);
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
    inline time_q BOND(const time_q& date) {
        return BOD(earlierOf(latestDate, AddOneDay(date)));
    }

    //------------------------------------------------------------------
    extern time_q fileLastModifyDate(const string_q& filename);

}  // namespace qblocks
