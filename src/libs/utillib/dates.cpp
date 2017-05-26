/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"

#include "dates.h"
#include "sfos.h"

namespace qblocks {

    //---------------------------------------------------------------------------------------
    const SFTime latestDate = SFTime(2200, 12, 31, 23, 59, 59);
    const SFTime earliestDate = SFTime(1700,  1,  1,  0,  0,  1);

    //---------------------------------------------------------------------------------------
    const uint32_t DaysPerMonth[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    //---------------------------------------------------------------------------------------
    bool isLeap(uint32_t year) {
        if ((year % 400) == 0)  // centuries every 400 years are leaps
            return true;

        if (((year % 4) == 0) && ((year % 100) != 0))  // otherwise every four years (but not centuries)
            return true;

        return false;
    }

    //---------------------------------------------------------------------------------------
    uint32_t DaysInMonth(uint32_t year, uint32_t month) {
        if (month == 2 && isLeap(year))
            return 29;
        return DaysPerMonth[month-1];
    }

    //---------------------------------------------------------------------------------------
    uint32_t DaysInMonth(const SFTime& date) {
        return DaysInMonth(date.GetYear(), date.GetMonth());
    }

    //---------------------------------------------------------------------------------------
    uint32_t DaysInYear(uint32_t year) {
        return isLeap(year) ? 366 : 365;
    }

    //---------------------------------------------------------------------------------------
    uint32_t DaysInYear(const SFTime& date) {
        return DaysInYear(date.GetYear());
    }

    //---------------------------------------------------------------------------------------
    SFTime AddOneDay(const SFTime& date) {
        if (date >= latestDate)
            return latestDate;

        uint32_t day    = date.GetDay();
        uint32_t month  = date.GetMonth();
        uint32_t year   = date.GetYear();
        uint32_t minute = date.GetMinute();
        uint32_t sec    = date.GetSecond();
        uint32_t hour   = date.GetHour();
        uint32_t nDays  = DaysInMonth(year, month);

        if (nDays == day) {
            if (month == 12) {
                month = 1;
                year  += 1;
                return SFTime(year, month, 1, hour, minute, sec);
            } else {
                month += 1;
                return SFTime(year, month, 1, hour, minute, sec);
            }
        }

        return SFTime(year, month, day+1, hour, minute, sec);
    }

    //---------------------------------------------------------------------------------------
    SFTime SubtractOneDay(const SFTime& date) {
        if (date <= earliestDate)
            return earliestDate;

        uint32_t day    = date.GetDay();
        uint32_t month  = date.GetMonth();
        uint32_t year   = date.GetYear();
        uint32_t minute = date.GetMinute();
        uint32_t sec    = date.GetSecond();
        uint32_t hour   = date.GetHour();

        if (day == 1) {
            if (month == 1) {
                month = 12;
                year  -= 1;
                return SFTime(year, month, DaysInMonth(year, month), hour, minute, sec);
            } else {
                month -= 1;
                return SFTime(year, month, DaysInMonth(year, month), hour, minute, sec);
            }
        }

        return SFTime(year, month, --day, hour, minute, sec);
    }

}  // namespace qblocks
