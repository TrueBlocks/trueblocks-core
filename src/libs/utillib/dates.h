#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "sftime.h"

namespace qblocks {

    #define FMT_JSON SFString("%Y-%m-%d %H:%M:%S UTC")

    extern uint32_t DaysInMonth(uint32_t year, uint32_t month);
    extern uint32_t DaysInMonth(const SFTime& date);
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

}  // namespace qblocks
