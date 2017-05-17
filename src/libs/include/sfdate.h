#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

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

    class SFDate {
    private:
        // Count of days since 15 October 1582 (start of Gregorian Calendar)
        SFUint32 m_nDays;

    public:
        SFDate(void);
        SFDate(const SFDate& dt);

    private:
        // Everything is private because on the SFTime class (which is exposed to end users and
        // is a 'friend' can use this class
        SFDate(uint32_t year, uint32_t month, uint32_t day);
        SFDate(uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek);
        explicit SFDate(int64_t days);
        explicit SFDate(const SF_TIMESTRUCT& sysTime);
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
        friend void SetDate(SFTime& date, const SFString& dStr);
    };

    //-------------------------------------------------------------------------
    inline uint32_t SFDate::GetDay() const {
        ASSERT(IsValid());
        return getDateStruct().m_Day;
    }

    //-------------------------------------------------------------------------
    inline uint32_t SFDate::GetMonth() const {
        ASSERT(IsValid());
        return getDateStruct().m_Month;
    }

    //-------------------------------------------------------------------------
    inline uint32_t SFDate::GetYear() const {
        ASSERT(IsValid());
        return getDateStruct().m_Year;
    }

    //-------------------------------------------------------------------------
    inline int64_t SFDate::GetTotalDays() const {
        ASSERT(IsValid());
        return m_nDays;
    }

    //-------------------------------------------------------------------------
    inline bool SFDate::IsValid() const {
        return (m_nDays != (SFUint32)LONG_MIN);
    }

    //-------------------------------------------------------------------------
    inline SFDate SFDate::operator+(int32_t days) const {
        int64_t res = m_nDays - 2000000000L;
        res += days;
        return SFDate(res);
    }

    //-------------------------------------------------------------------------
    inline SFDate SFDate::operator-(int32_t days) const {
        return SFDate((m_nDays - 2000000000L) - days);
    }

    //-------------------------------------------------------------------------
    inline SFDate& SFDate::operator+=(int32_t days) {
        *this = *this + days;
        return *this;
    }

    //-------------------------------------------------------------------------
    inline SFDate& SFDate::operator-=(int32_t days) {
        *this = *this - days;
        return *this;
    }

    //-------------------------------------------------------------------------
    inline bool SFDate::operator==(const SFDate& date) const {
        return (m_nDays == date.m_nDays);
    }

    //-------------------------------------------------------------------------
    inline bool SFDate::operator!=(const SFDate& date) const {
        return !operator==(date);
    }

    //-------------------------------------------------------------------------
    inline bool SFDate::operator>(const SFDate& date) const {
        return m_nDays > date.m_nDays;
    }
    
    //-------------------------------------------------------------------------
    inline bool SFDate::operator<(const SFDate& date) const {
        return m_nDays < date.m_nDays;
    }
    
    //-------------------------------------------------------------------------
    inline bool SFDate::operator>=(const SFDate& date) const {
        return m_nDays >= date.m_nDays;
    }
    
    //-------------------------------------------------------------------------
    inline bool SFDate::operator<=(const SFDate& date) const {
        return m_nDays <= date.m_nDays;
    }
}
