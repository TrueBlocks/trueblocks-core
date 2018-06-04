#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "conversions_base.h"

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

            // Everything is private because on the SFTime class (which is exposed to end users and
            // is a 'friend' can use this class
            SFDate(uint32_t year, uint32_t month, uint32_t day);
            SFDate(uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek);
            explicit SFDate(int64_t days);
            explicit SFDate(const tm& sysTime);
            SFDate(const SFString& dateStr, const SFString& fmtStr);

            // ~SFDate(void);

            SFDate& operator=(const SFDate& date);

            uint32_t GetYear(void) const;
            uint32_t GetMonth(void) const;
            uint32_t GetDay(void) const;

            int64_t GetTotalDays(void) const;

            SFDate operator+(int32_t days) const;
            SFDate operator-(int32_t days) const;

            SFDate& operator+=(int32_t days);
            SFDate& operator-=(int32_t days);

            bool operator==(const SFDate& date) const;
            bool operator!=(const SFDate& date) const;

            bool operator>(const SFDate& date) const;
            bool operator<(const SFDate& date) const;

            bool operator>=(const SFDate& date) const;
            bool operator<=(const SFDate& date) const;

            SFString Format(const SFString& fmt = "") const;

            bool IsValid(void) const;

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
            SFTimeOfDay(void);
            SFTimeOfDay(const SFTimeOfDay& tod);

            // Everything is private because on the SFTime class (which is exposed to
            // end users and is a 'friend' can use this class)
            explicit SFTimeOfDay(const tm& sysTime);
            explicit SFTimeOfDay(uint32_t secs);
            SFTimeOfDay(uint32_t h, uint32_t m, uint32_t s);
            SFTimeOfDay(const SFString& dateStr, const SFString& fmtStr);

            // ~SFTimeOfDay(void);
            SFTimeOfDay&  operator=(const SFTimeOfDay& tod);

            uint32_t GetHour(void) const;
            uint32_t GetMinute(void) const;
            uint32_t GetSecond(void) const;
            uint32_t GetTotalSeconds(void) const;

            bool operator==(const SFTimeOfDay& tod) const;
            bool operator!=(const SFTimeOfDay& tod) const;

            bool operator>(const SFTimeOfDay& tod) const;
            bool operator<(const SFTimeOfDay& tod) const;

            bool operator>=(const SFTimeOfDay& tod) const;
            bool operator<=(const SFTimeOfDay& tod) const;

            SFString Format(const SFString& fmt = "") const;

            bool IsValid(void) const;

            uint32_t m_nSeconds;
            friend class SFTime;
        };

    public:
        SFTime(void);
        SFTime(const SFTime& date);

        SFTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t sec);
        SFTime(uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek,
               uint32_t hour, uint32_t minute, uint32_t sec);
        SFTime(uint32_t days, uint32_t hour, uint32_t minute, uint32_t sec);
        SFTime(const SFDate& date, const SFTimeOfDay& tod);
        explicit SFTime(const tm& sysTime, bool useDayOfWeek = false);
        SFTime(const SFString& dateStr, const SFString& fmtStr);

        // ~SFTime(void);

        SFTime& operator=(const SFTime& date);

        uint32_t GetDay(void) const;
        uint32_t GetMonth(void) const;
        uint32_t GetYear(void) const;
        uint32_t GetHour(void) const;
        uint32_t GetMinute(void) const;
        uint32_t GetSecond(void) const;
        uint32_t GetDayOfWeek(void) const;

        timestamp_t GetTotalSeconds(void) const;

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

        bool onTheHour(void) const;

        SFString Format(const SFString& fmt) const;

        SFDate getDatePart(void) const;
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
}  // namespace qblocks
