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

    class SFTime;
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
    class SFTime {
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

            friend class SFTime;
            friend uint32_t getWeekOfMonth(const SFDate& date);
            friend uint32_t getDayOfWeek(const SFDate& date);
        };

        //----------------------------------------------------------------------------
        // Used by SFTime to hold the time portion of the date
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
        SFTime(void);
        SFTime(const SFTime& date);

        SFTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t sec);
        SFTime(uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek,
                            uint32_t hour, uint32_t minute, uint32_t sec);
        SFTime(uint32_t days, uint32_t hour, uint32_t minute, uint32_t sec);
        SFTime(const SFDate& date, const SFTimeOfDay& tod);
        SFTime(const string_q& dateStr, const string_q& fmtStr);

        explicit SFTime(const tm& sysTime, bool useDayOfWeek = false);

        SFTime& operator=(const SFTime& date);

        bool IsValid(void) const;

        SFTime operator+(const uint32_t& ts) const;
        SFTime operator-(const uint32_t& ts) const;
        int64_t operator-(SFTime& date);

        bool operator==(const SFTime& date) const;
        bool operator!=(const SFTime& date) const;

        bool operator>(const SFTime& date) const;
        bool operator<(const SFTime& date) const;

        bool operator>=(const SFTime& date) const;
        bool operator<=(const SFTime& date) const;

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
    extern SFTime Now(void);

    //---------------------------------------------------------------------------------------------
    extern ostream &operator <<(ostream &os, const SFTime& x);

    extern const SFTime latestDate;
    extern const SFTime earliestDate;

    typedef SFArrayBase<SFTime> SFTimeArray;

#define FMT_JSON string_q("%Y-%m-%d %H:%M:%S UTC")

    extern uint32_t DaysInMonth(uint32_t year, uint32_t month);
    extern SFTime   AddOneDay(const SFTime& date);
    extern SFTime   SubtractOneDay(const SFTime& date);

    inline SFTime BOH(const SFTime& date) {
        // H:00:00
        return SFTime(date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), 0, 0);
    }

    inline SFTime EOH(const SFTime& date) {
        // H:59:59
        return SFTime(date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), 59, 59);
    }

    inline SFTime BOD(const SFTime& date) {
        // 12:00:00 am
        return SFTime(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
    }

    inline SFTime EOD(const SFTime& date) {
        // 11:59:59 pm
        return SFTime(date.GetYear(), date.GetMonth(), date.GetDay(), 23, 59, 59);
    }

    extern SFTime BOW(const SFTime& tm);
    extern SFTime EOW(const SFTime& tm);

    inline SFTime MIDDAY(const SFTime& date) {
        // 12:00 noon
        return SFTime(date.GetYear(), date.GetMonth(), date.GetDay(), 12, 0, 0);
    }

    inline bool isInRange(const SFTime& ref, const SFTime& start, const SFTime& end) {
        return (start <= ref && end >= ref);
    }

    inline SFTime earlierOf(const SFTime& one, const SFTime& two) {
        if (one < two)
            return one;
        else if (two < one)
            return two;
        return one;
    }

    inline SFTime laterOf(const SFTime& one, const SFTime& two) {
        if (one > two)
            return one;
        else if (two > one)
            return two;
        return one;
    }

    inline uint32_t getCentury(uint32_t year) {
        return ((year / 100) * 100);
    }

    inline uint32_t get2Digit(uint32_t year) {
        return year - getCentury(year);
    }

    //---------------------------------------------------------------------------------------------
    inline SFTime BOND(const SFTime& date) {
        return BOD(earlierOf(latestDate, AddOneDay(date)));
    }

    //--------------------------------------------------------------------------------------------------------------
    extern SFTime dateFromTimeStamp(timestamp_t tsIn);
    extern SFTime parseDate(const string_q& str);

    //--------------------------------------------------------------------------------------------------------------
    extern timestamp_t toTimestamp(const SFTime&   timeIn);
    extern timestamp_t toTimestamp(const string_q& timeIn);
    extern string_q    fromTimestamp(timestamp_t ts);

    //------------------------------------------------------------------
    extern SFTime fileLastModifyDate(const string_q& filename);

}  // namespace qblocks
