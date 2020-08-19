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
#include <algorithm>
#include <string>
#include "basetypes.h"
#include "sftime.h"
#include "sfos.h"
#include "conversions.h"
#include "logging.h"

namespace qblocks {

#define SECS_PER_DAY ((unsigned int)(24 * 60 * 60))
#define SECS_PER_HOUR ((unsigned int)(60 * 60))
#define SECS_PER_MIN ((unsigned int)(60))

//----------------------------------------------------------------------------------------------------
time_q::time_q() {
    m_nSeconds = 0xdeadbeef;
}

//----------------------------------------------------------------------------------------------------
time_q::time_q(const time_q& date) {
    m_nSeconds = date.m_nSeconds;
}

//----------------------------------------------------------------------------------------------------
time_q::time_q(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t sec) {
    *this = time_q(CDate(year, month, day), CTimeOfDay(hour, minute, sec));
}

//----------------------------------------------------------------------------------------------------
time_q::time_q(uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek, uint32_t hour, uint32_t minute,
               uint32_t sec) {
    *this = time_q(CDate(year, month, weekInMonth, dayOfWeek), CTimeOfDay(hour, minute, sec));
}

//----------------------------------------------------------------------------------------------------
time_q::time_q(uint32_t days, uint32_t hour, uint32_t minute, uint32_t sec) {
    *this = time_q(CDate(days), CTimeOfDay(hour, minute, sec));
}

//----------------------------------------------------------------------------------------------------
time_q::time_q(const CDate& datePart, const CTimeOfDay& timePart) {
    m_nSeconds = (datePart.GetTotalDays() * SECS_PER_DAY) + timePart.GetTotalSeconds();
}

//----------------------------------------------------------------------------------------------------
time_q::time_q(const string_q& dateStr, const string_q& fmtStr) {
    if (dateStr.empty() || fmtStr.empty())
        *this = earliestDate;
    else
        *this = time_q(CDate(dateStr, fmtStr), CTimeOfDay(dateStr, fmtStr));
}

//----------------------------------------------------------------------------------------------------
time_q::time_q(const tm& st, bool useDayOfWeek) {
    tm sysTime = st;
    if (!sysTime.tm_year)
        sysTime.tm_year = static_cast<int>(Now().GetYear());
    ASSERT(sysTime.tm_year);

    if (useDayOfWeek) {
        *this = time_q(CDate((uint32_t)sysTime.tm_year, (uint32_t)sysTime.tm_mon, (uint32_t)sysTime.tm_mday,
                             (uint32_t)(sysTime.tm_wday + 1)),
                       CTimeOfDay(sysTime));
    } else {
        *this = time_q(CDate(sysTime), CTimeOfDay(sysTime));
    }
}

//----------------------------------------------------------------------------------------------------
time_q& time_q::operator=(const time_q& d) {
    m_nSeconds = d.m_nSeconds;
    return *this;
}

//----------------------------------------------------------------------------------------------------
// %a    Abbreviated weekday name
// %A    Full weekday name
// %b    Abbreviated month name
// %B    Full month name
// %d    Day of month as decimal number (01 - 31)
// %f    Calendar being used "O.S" Old Style (Julian) or "N.S" New Style (Gregorian)
// %j    Day of year as decimal number (001 - 366)
// %m    Month as decimal number (01 - 12)
// %P    AM/PM indicator
// %p    a/p indicator
// %U    Week of year as decimal number
// %w    Weekday as decimal number (1 - 7; Sunday is 1)
// %x    Date representation for current locale,  namely
//         Date representation + " " + Time Representation for current locale
// %y    Year without century, as decimal number (00 - 99)
// %Y    Year with century, as decimal number
// %c    Year displayed using C.E.(Current Epoch) / B.C.E (Before Current Epoch) convention e.g. -1023 = 1022 BCE
//
// %H    Hours in the day
// %h    12 Hour format Hours in (00 - 12)
// %M    Minutes in the hour
// %S    Seconds in the minute
// %Q    Seconds so far this day
// %%    Percent sign
//
//
// may also need to include full windows escape character
//
// %%    Percent sign
//
// As in the printf function, the # flag may prefix any formatting code.
// In that case, the meaning of the format code is changed as follows.
//
// Format Code Meaning
// %#x Long date representation, appropriate to current locale, namely
//       Long Date representation + " " + Time Representation for current locale
//
//
// %#d, %#j, %#m, %#U, %#w, %#y, %#H, %#h, %#M, %#S  Remove leading zeros (if any).
//----------------------------------------------------------------------------------------------------
string_q time_q::Format(const string_q& sFormat) const {
    string_q ret;
    if (IsValid()) {
        char sBuffer[512];
        size_t bSize = sizeof(sBuffer);

        size_t sFmtLength = sFormat.length();
        for (size_t i = 0; i < sFmtLength; i++) {
            char c = sFormat.at(i);
            if (c == '%') {
                ++i;
                if (i < sFmtLength) {
                    c = sFormat.at(i);
                    switch (c) {
                        case 'Q': {
                            uint32_t hour = GetHour();
                            uint32_t minute = GetMinute();
                            uint32_t secs = GetSecond();
                            ret += int_2_Str(hour * 60 * 60 + minute * 60 + secs);
                            break;
                        }
                        case 'H': {
                            snprintf(sBuffer, bSize, "%.02d", GetHour());
                            ret += sBuffer;
                            break;
                        }
                        case 'h': {
                            uint32_t h = GetHour();
                            if (h > 12)
                                h -= 12;
                            if (h == 0)
                                h = 12;
                            snprintf(sBuffer, bSize, "%.02d", h);
                            ret += sBuffer;
                            break;
                        }
                        case 'M': {
                            snprintf(sBuffer, bSize, "%.02d", GetMinute());
                            ret += sBuffer;
                            break;
                        }
                        case 'P': {
                            ASSERT(getTimePart().IsValid());
                            ret += (GetHour() >= 12 ? "pm" : "am");
                            break;
                        }
                        case 'p': {
                            ASSERT(getTimePart().IsValid());
                            ret += (GetHour() >= 12 ? "p" : "a");
                            break;
                        }
                        case 'S': {
                            snprintf(sBuffer, bSize, "%.02d", GetSecond());
                            ret += sBuffer;
                            break;
                        }
                        case 'd': {
                            snprintf(sBuffer, bSize, "%.02d", GetDay());
                            ret += sBuffer;
                            break;
                        }
                        case 'f': {
                            ret += "Gregorian Calendar";
                            break;
                        }
                        case 'm': {
                            snprintf(sBuffer, bSize, "%.02d", GetMonth());
                            ret += sBuffer;
                            break;
                        }
                        case 'y': {
                            snprintf(sBuffer, bSize, "%.02d", get2Digit(GetYear()));
                            ret += sBuffer;
                            break;
                        }
                        case 'Y': {
                            snprintf(sBuffer, bSize, "%d", GetYear());
                            ret += sBuffer;
                            break;
                        }
                        case '#': {
                            if (i < sFmtLength) {
                                ++i;
                                c = sFormat.at(i);
                                switch (c) {
                                    case 'd': {
                                        snprintf(sBuffer, bSize, "%d", GetDay());
                                        ret += sBuffer;
                                        break;
                                    }
                                    case 'm': {
                                        snprintf(sBuffer, bSize, "%d", GetMonth());
                                        ret += sBuffer;
                                        break;
                                    }
                                    case 'y': {
                                        snprintf(sBuffer, bSize, "%d", get2Digit(GetYear()));
                                        ret += sBuffer;
                                        break;
                                    }
                                    case 'H': {
                                        snprintf(sBuffer, bSize, "%d", GetHour());
                                        ret += sBuffer;
                                        break;
                                    }
                                    case 'h': {
                                        uint32_t h = GetHour();
                                        if (h > 12)
                                            h -= 12;
                                        if (h == 0)
                                            h = 12;
                                        snprintf(sBuffer, bSize, "%d", h);
                                        ret += sBuffer;
                                        break;
                                    }
                                    case 'M': {
                                        snprintf(sBuffer, bSize, "%d", GetMinute());
                                        ret += sBuffer;
                                        break;
                                    }
                                    case 'S': {
                                        snprintf(sBuffer, bSize, "%d", GetSecond());
                                        ret += sBuffer;
                                        break;
                                    }
                                    default: {
                                        ret += c;
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        default: {
                            ret += c;
                            break;
                        }
                    }
                }
            } else {
                ret += c;
            }
        }
    }

    return ret;
}

//----------------------------------------------------------------------------------------------------
ostream& operator<<(ostream& os, const time_q& x) {
    os << x.Format(FMT_JSON);
    return os;
}

//-------------------------------------------------------------------------
uint32_t time_q::CTimeOfDay::GetHour() const {
    ASSERT(IsValid());
    return (uint32_t)(m_nSeconds / SECS_PER_HOUR);
}

//-------------------------------------------------------------------------
uint32_t time_q::CTimeOfDay::GetMinute() const {
    ASSERT(IsValid());
    uint32_t secsInHours = GetHour() * SECS_PER_HOUR;
    return (uint32_t)((m_nSeconds - secsInHours) / SECS_PER_MIN);
}

//-------------------------------------------------------------------------
uint32_t time_q::CTimeOfDay::GetSecond() const {
    ASSERT(IsValid());
    uint32_t secsInHours = GetHour() * SECS_PER_HOUR;
    uint32_t secsInMins = GetMinute() * SECS_PER_MIN;
    return (m_nSeconds - secsInHours - secsInMins);
}

//-------------------------------------------------------------------------
uint32_t time_q::CTimeOfDay::GetTotalSeconds() const {
    ASSERT(IsValid());
    return m_nSeconds;
}

//-------------------------------------------------------------------------
uint32_t time_q::GetYear() const {
    return getDatePart().GetYear();
}

//-------------------------------------------------------------------------
uint32_t time_q::GetMonth() const {
    return getDatePart().GetMonth();
}

//-------------------------------------------------------------------------
uint32_t time_q::GetDay() const {
    return getDatePart().GetDay();
}

//-------------------------------------------------------------------------
uint32_t time_q::GetHour() const {
    return getTimePart().GetHour();
}

//-------------------------------------------------------------------------
uint32_t time_q::GetMinute() const {
    return getTimePart().GetMinute();
}

//-------------------------------------------------------------------------
uint32_t time_q::GetSecond() const {
    return getTimePart().GetSecond();
}

//-------------------------------------------------------------------------
timestamp_t time_q::GetTotalSeconds(void) const {
    return m_nSeconds;
}

//-------------------------------------------------------------------------
extern uint32_t getDayOfWeek(const time_q::CDate& date);

//-------------------------------------------------------------------------
// We only do the test for equality and greater than. We then use these
// to do all other tests
bool time_q::operator==(const time_q& date) const {
    ASSERT(IsValid());
    return (m_nSeconds == date.m_nSeconds);
}

//-------------------------------------------------------------------------
bool time_q::operator!=(const time_q& date) const {
    return !operator==(date);
}

//-------------------------------------------------------------------------
bool time_q::operator>(const time_q& date) const {
    return (m_nSeconds > date.m_nSeconds);
}

//-------------------------------------------------------------------------
bool time_q::operator>=(const time_q& date) const {
    return operator>(date) || operator==(date);
}

//-------------------------------------------------------------------------
bool time_q::operator<(const time_q& date) const {
    return (m_nSeconds < date.m_nSeconds);
}

//-------------------------------------------------------------------------
bool time_q::operator<=(const time_q& date) const {
    return operator<(date) || operator==(date);
}

//-------------------------------------------------------------------------
time_q time_q::operator+(const uint32_t& ts) const {
    time_q ret;
    ret.m_nSeconds = m_nSeconds + ts;
    return ret;
}

//-------------------------------------------------------------------------
time_q time_q::operator-(const uint32_t& ts) const {
    time_q ret;
    ret.m_nSeconds = m_nSeconds - ts;
    return ret;
}

//-------------------------------------------------------------------------
int64_t time_q::operator-(time_q& date) {
    return m_nSeconds - date.m_nSeconds;
}

//-------------------------------------------------------------------------
bool time_q::IsValid() const {
    return (m_nSeconds != (int64_t)0xdeadbeef);
}

//-------------------------------------------------------------------------
time_q::CDate time_q::getDatePart() const {
    ASSERT(IsValid());
    return CDate((m_nSeconds / SECS_PER_DAY) - 2000000000L);
}

//-------------------------------------------------------------------------
time_q::CTimeOfDay time_q::getTimePart() const {
    if (m_nSeconds == getDatePart().GetTotalDays())
        return CTimeOfDay(0);  // midnight at start of day

    ASSERT(IsValid());
    ASSERT(m_nSeconds >= getDatePart().GetTotalDays());

    int64_t secs = m_nSeconds;
    int64_t dateSecs = getDatePart().GetTotalDays() * SECS_PER_DAY;
    return CTimeOfDay(uint32_t(secs - dateSecs));
}

//-------------------------------------------------------------------------
bool time_q::onTheHour(void) const {
    return (GetMinute() < 5 || GetMinute() > 55);
}

//-------------------------------------------------------------------------
time_q::CTimeOfDay::CTimeOfDay() {
    // create an invalid value (would be the next day otherwise!)
    m_nSeconds = SECS_PER_DAY;
}

//-------------------------------------------------------------------------
time_q::CTimeOfDay::CTimeOfDay(const CTimeOfDay& tod) {
    m_nSeconds = tod.m_nSeconds;
}

//-------------------------------------------------------------------------
time_q::CTimeOfDay::CTimeOfDay(uint32_t h, uint32_t m, uint32_t s) {
    m_nSeconds = h * SECS_PER_HOUR + m * SECS_PER_MIN + s;
    m_nSeconds = min(SECS_PER_DAY, m_nSeconds);  // make it invalid of overrun
}

//-------------------------------------------------------------------------
time_q::CTimeOfDay::CTimeOfDay(uint32_t secs) {
    m_nSeconds = secs;
    m_nSeconds = min(SECS_PER_DAY, m_nSeconds);  // make it invalid of overrun
}

//-------------------------------------------------------------------------
time_q::CTimeOfDay::CTimeOfDay(const tm& sysTime) {
    m_nSeconds = ((uint32_t)sysTime.tm_hour) * SECS_PER_HOUR + (uint32_t)(sysTime.tm_min) * SECS_PER_MIN +
                 (uint32_t)(sysTime.tm_sec);
    m_nSeconds = min(SECS_PER_DAY, m_nSeconds);  // make it invalid of overrun
}

//-------------------------------------------------------------------------
//
// dateStr is a legal date string as defined by fmtStr.
//
// fmtStr is a 4 character string where the first 3 indicate the
// order of the hour/minute/sec and the fourth character indicates the separator.
// The date string should have a space between the time and meridian (if
// meridian is not present then assume AM.  If hour > 12 then assume military
//
// For example:
//
// fmtStr dateStr Result
//
// "hms:" "12:10 " 12:10 pm
// "hms-" "0-10-0" 12:10:00 am
//
// This feature allows for the creation of CTimeOfDay's from a string
// as entered in a masked edit field or from a parsed report.
//
//-------------------------------------------------------------------------
time_q::CTimeOfDay::CTimeOfDay(const string_q& timeStr, const string_q& fmtStr) {
    m_nSeconds = SECS_PER_DAY;
    if (fmtStr.length() != 4) {
        *this = CTimeOfDay(12, 0, 0);
        return;
    }

    uint32_t hour = 12;
    uint32_t minute = 0;
    uint32_t secs = 0;
    bool am = true;

    char sep = fmtStr[3];

    string_q str = timeStr;  // 12:12:12 am for example

    for (size_t i = 0; i < 3 && str.length() > 0; i++) {
        switch (fmtStr[i]) {
            case 'h':
            case 'H':
                hour = (uint32_t)str_2_Uint(nextTokenClear(str, sep));
                break;

            case 'm':
            case 'M':
                minute = (uint32_t)str_2_Uint(nextTokenClear(str, sep));
                break;

            case 's':
            case 'S':
                secs = (uint32_t)str_2_Uint(nextTokenClear(str, sep));
                break;
        }
    }

    size_t find = timeStr.find(" ");
    if (find != string::npos) {
        str = toLower(extract(timeStr, find));
        if (contains(str, "p"))
            am = false;
    }

    if (!am && hour < 12)
        hour += 12;

    else if (am && hour == 12)
        hour = 0;

    *this = CTimeOfDay(hour, minute, secs);
}

//-------------------------------------------------------------------------
uint32_t time_q::CDate::GetDay() const {
    ASSERT(IsValid());
    return getDateStruct().m_Day;
}

//-------------------------------------------------------------------------
uint32_t time_q::CDate::GetMonth() const {
    ASSERT(IsValid());
    return getDateStruct().m_Month;
}

//-------------------------------------------------------------------------
uint32_t time_q::CDate::GetYear() const {
    ASSERT(IsValid());
    return getDateStruct().m_Year;
}

//-------------------------------------------------------------------------
int64_t time_q::CDate::GetTotalDays() const {
    ASSERT(IsValid());
    return (int64_t)m_nDays;
}

//-------------------------------------------------------------------------
time_q::CDate time_q::CDate::operator+(int32_t days) const {
    int64_t res = ((int64_t)m_nDays) - 2000000000L;
    res += days;
    return CDate(res);
}

//-------------------------------------------------------------------------
time_q::CDate time_q::CDate::operator-(int32_t days) const {
    return CDate(int64_t(m_nDays - 2000000000L) - days);
}

//-------------------------------------------------------------------------
time_q::CDate& time_q::CDate::operator+=(int32_t days) {
    *this = *this + days;
    return *this;
}

//-------------------------------------------------------------------------
time_q::CDate& time_q::CDate::operator-=(int32_t days) {
    *this = *this - days;
    return *this;
}

//-------------------------------------------------------------------------
time_q::CDate::CDate(void) {
    m_nDays = (uint64_t)LONG_MIN;
}

//-------------------------------------------------------------------------
time_q::CDate::CDate(const CDate& date) {
    m_nDays = date.m_nDays;
}

//-------------------------------------------------------------------------
time_q::CDate::CDate(uint32_t y, uint32_t m, uint32_t d) {
    setValues(y, m, d);
}

//-------------------------------------------------------------------------
time_q::CDate::CDate(uint32_t y, uint32_t m, uint32_t weekInMonth, uint32_t dayOfWeek) {
    // assume it fails
    m_nDays = (uint64_t)LONG_MIN;

    // assume any year is OK
    if ((m >= 1 && m <= 12) && (weekInMonth >= 1 && weekInMonth <= 4) && (dayOfWeek >= 1 && dayOfWeek <= 7)) {
        CDate firstDay(y, m, 1);
        ASSERT(firstDay.IsValid());

        uint32_t dow = getDayOfWeek(firstDay);
        uint32_t d = (((dayOfWeek - dow + 7) % 7) + (7 * (weekInMonth - 1)) + 1);
        setValues(y, m, d);
    }
}

//-------------------------------------------------------------------------
time_q::CDate::CDate(int64_t days) {
    if (days > 2146905911L)  // Largest valid GD N
        m_nDays = (uint64_t)LONG_MIN;
    else
        m_nDays = (uint64_t)(((int64_t)days) + 2000000000L);
}

//-------------------------------------------------------------------------
time_q::CDate::CDate(const tm& sysTime) {
    setValues((uint32_t)sysTime.tm_year, (uint32_t)sysTime.tm_mon, (uint32_t)sysTime.tm_mday);
}

//-------------------------------------------------------------------------
//
// dateStr is a legal date string as defined by fmtStr.
//
// fmtStr is a 5 character string where the first 3 indicate the
// order of the month/day/year, the fourth character indicates the length
// of the year, and the fifth character indicates the separator.
//
// For example:
//
// fmtStr dateStr Result
//
// "dmy2/" "20/03/96" 20th March 1996
// "mdy4-" "03-28-1996" 28th March 1996
//
// This feature allows for the creation of CDate's from a string
// as entered in a masked edit field or from a parsed report.
//-------------------------------------------------------------------------
time_q::CDate::CDate(const string_q& dateStr, const string_q& fmtStr) {
    m_nDays = (uint64_t)LONG_MIN;
    if (fmtStr.length() != 5) {
        *this = CDate(Now().GetYear(), Now().GetMonth(), Now().GetDay());
        return;
    }

    uint32_t day = 1;
    uint32_t month = 1;
    uint32_t year = Now().GetYear();

    char sep = fmtStr[4];

    string_q str = dateStr;  // 12-10-1921 for example
    for (size_t i = 0; i < 3 && str.length() > 0; i++) {
        switch (fmtStr[i]) {
            case 'd':
            case 'D':
                day = (uint32_t)str_2_Uint(nextTokenClear(str, sep));
                break;
            case 'm':
            case 'M':
                month = (uint32_t)str_2_Uint(nextTokenClear(str, sep));
                break;
            case 'y':
            case 'Y': {
                year = (uint32_t)str_2_Uint(nextTokenClear(str, sep));
                char c = fmtStr.at(3);
                ASSERT((c == '2') || (c == '4'));
                if (c == '2' || year < 100) {
                    if (year >= 50)
                        year += 1900;
                    else if (year < 50)
                        year += 2000;
                }
            } break;
        }
    }

#define CLAMP(_vv, _ll, _hh) min((uint32_t)_hh, max((uint32_t)_ll, (uint32_t)_vv))
    year = CLAMP(year, earliestDate.GetYear(), latestDate.GetYear());
    month = CLAMP(month, 1, 12);
    day = CLAMP(day, 1, DaysInMonth(year, month));

    setValues(year, month, day);
}

//-------------------------------------------------------------------------
time_q::CDate& time_q::CDate::operator=(const CDate& date) {
    m_nDays = date.m_nDays;
    return *this;
}

//-------------------------------------------------------------------------
template <class TYPE>
TYPE lfloor(TYPE a, TYPE b) {
    ASSERT(b > 0);
    return (a >= 0 ? a / b : (a % b == 0) - 1 - ((TYPE)labs(a)) / b);
}

//-------------------------------------------------------------------------
time_q::CDate& time_q::CDate::setValues(uint32_t y, uint32_t m, uint32_t d) {
    m_nDays = (uint64_t)LONG_MIN;
    if (m >= JANUARY && m <= DECEMBER && d <= DaysInMonth(y, m)) {
        // The following algorithm has been taken from from an article in
        // the March 1993 issue of the Windows / Dos Developers Journal.
        m_nDays = (y - 1) * 365 + (uint64_t)lfloor((int32_t)(y - 1), 4);

        m_nDays += (uint64_t)lfloor((int32_t)(y - 1), 400) - (uint64_t)lfloor((int32_t)(y - 1), 100);

        --m;
        while (m) {
            m_nDays += DaysInMonth(y, m);
            m--;
        }
        m_nDays += (d + 1999422264L);  // ensure all usable date values are positive by adding 2 billion to m_nDays
    }
    return *this;
}

//-------------------------------------------------------------------------
CDateStruct time_q::CDate::getDateStruct() const {
    // See the comment in CDate::setValues(uint32_t Year, uint32_t Month, uint32_t Day)
    // for references to where the algorithm is taken from
    ASSERT(IsValid());

    CDateStruct ds;
    int64_t gdn = int64_t(m_nDays - 1999422265);
    int64_t y4 = 1461;
    int64_t y400 = 146100 - 3;
    int64_t y100 = 36525 - 1;

    bool exception = false;
    ds.m_Year = (uint32_t)(400 * lfloor(gdn, y400));
    gdn -= y400 * lfloor(gdn, y400);  // 400 year periods

    if (gdn > 0L) {
        ds.m_Year += 100 * lfloor(gdn, y100);  // 100 year periods
        gdn -= y100 * lfloor(gdn, y100);
        exception = (gdn == 0 && true);
        if (gdn > 0) {
            ds.m_Year += 4 * lfloor(gdn, y4);  // 4 year periods
            gdn -= y4 * lfloor(gdn, y4);
            if (gdn > 0) {
                int i = 0;
                while (gdn > 365 && ++i < 4) {
                    ds.m_Year++;
                    gdn -= 365;
                }
            }
        }
    }

    if (exception) {
        gdn = 366;  // occurs once every hundred years with Gregorian calendar

    } else {
        ds.m_Year++;
        gdn++;
    }

    ds.m_Month = 1;
    while (ds.m_Month < 13 && gdn > DaysInMonth(ds.m_Year, ds.m_Month))
        gdn -= DaysInMonth(ds.m_Year, ds.m_Month++);

    if (ds.m_Month == 13) {
        ds.m_Month = 1;
        ds.m_Year++;
    }

    ds.m_Day = (uint32_t)gdn;
    return ds;
}

//---------------------------------------------------------------------------------------
uint32_t getDayOfWeek(const time_q::CDate& date) {
    ASSERT(date.IsValid());
    return (date.GetTotalDays() % 7) + 1;
}

//---------------------------------------------------------------------------------------
time_q Now(void) {
    time_t loc;
    time(&loc);

    tm unused;
    struct tm* ret = localtime_r(&loc, &unused);
    if (ret) {
        tm sysTime = *ret;
        sysTime.tm_year += 1900;
        sysTime.tm_mon += 1;
        return time_q(sysTime, false);
    }
    return earliestDate;
}

//---------------------------------------------------------------------------------------
const time_q latestDate = time_q(2200, 12, 31, 23, 59, 59);
const time_q earliestDate = time_q(1700, 1, 1, 0, 0, 1);

//---------------------------------------------------------------------------------------
const uint32_t DaysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

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
    return DaysPerMonth[month - 1];
}

//---------------------------------------------------------------------------------------
uint32_t DaysInYear(uint32_t year) {
    return isLeap(year) ? 366 : 365;
}

//---------------------------------------------------------------------------------------
uint32_t DaysInYear(const time_q& date) {
    return DaysInYear(date.GetYear());
}

//---------------------------------------------------------------------------------------
time_q AddOneDay(const time_q& date) {
    if (date >= latestDate)
        return latestDate;

    uint32_t day = date.GetDay();
    uint32_t month = date.GetMonth();
    uint32_t year = date.GetYear();
    uint32_t minute = date.GetMinute();
    uint32_t sec = date.GetSecond();
    uint32_t hour = date.GetHour();
    uint32_t nDays = DaysInMonth(year, month);

    if (nDays == day) {
        if (month == 12) {
            month = 1;
            year += 1;
            return time_q(year, month, 1, hour, minute, sec);
        } else {
            month += 1;
            return time_q(year, month, 1, hour, minute, sec);
        }
    }

    return time_q(year, month, day + 1, hour, minute, sec);
}

//---------------------------------------------------------------------------------------
time_q SubtractOneDay(const time_q& date) {
    if (date <= earliestDate)
        return earliestDate;

    uint32_t day = date.GetDay();
    uint32_t month = date.GetMonth();
    uint32_t year = date.GetYear();
    uint32_t minute = date.GetMinute();
    uint32_t sec = date.GetSecond();
    uint32_t hour = date.GetHour();

    if (day == 1) {
        if (month == 1) {
            month = 12;
            year -= 1;
            return time_q(year, month, DaysInMonth(year, month), hour, minute, sec);
        } else {
            month -= 1;
            return time_q(year, month, DaysInMonth(year, month), hour, minute, sec);
        }
    }

    return time_q(year, month, --day, hour, minute, sec);
}

//---------------------------------------------------------------------------------------------
time_q AddOneHour(const time_q& date) {
    if (BOH(date).GetHour() == 23)
        return BOD(AddOneDay(date));
    return time_q(date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour() + 1, 0, 0);
}

//---------------------------------------------------------------------------------------------
time_q AddOneWeek(const time_q& date) {
    time_q ret = date;
    for (size_t i = 0; i < 7; i++)
        ret = AddOneDay(ret);
    return ret;
}

//---------------------------------------------------------------------------------------------
time_q AddOneMonth(const time_q& date) {
    if (date.GetMonth() == 12)
        return time_q(date.GetYear() + 1, 1, 1, 0, 0, 0);
    return time_q(date.GetYear(), date.GetMonth() + 1, 1, 0, 0, 0);
}

//---------------------------------------------------------------------------------------------
time_q AddOneQuarter(const time_q& date) {
    if (date.GetMonth() > 9)
        return time_q(date.GetYear() + 1, ((date.GetMonth() + 3) % 12) + 1, 1, 0, 0, 0);
    return time_q(date.GetYear(), date.GetMonth() + 3, 1, 0, 0, 0);
}

//---------------------------------------------------------------------------------------------
time_q AddOneYear(const time_q& date) {
    return time_q(date.GetYear() + 1, 1, 1, 0, 0, 0);
}

//----------------------------------------------------------------------------------------------------
time_q BOW(const time_q& tm) {
    time_q ret = BOD(tm);
    while (getDayOfWeek(ret.getDatePart()) > 1)  // if it equals '1', it's Sunday at 00:00:01
        ret = SubtractOneDay(ret);
    return ret;
}

//----------------------------------------------------------------------------------------------------
time_q EOW(const time_q& tm) {
    time_q ret = EOD(tm);
    while (getDayOfWeek(tm.getDatePart()) < 7)  // if it equals '7', it's Saturday 12:59:59
        ret = AddOneDay(ret);
    return ret;
}

//------------------------------------------------------------------------
typedef time_q (*PTF)(const time_q& date);

//------------------------------------------------------------------------
bool expandTimeArray(CTimeArray& ta, const time_q& startIn, const time_q& stop, bool fallback, PTF BOP, PTF BONP) {
    if (fallback)
        ta.push_back(BOP(startIn));
    for (time_q t = BONP(startIn); t <= BONP(stop);) {
        ta.push_back(t);
        t = BONP(t);
    }
    return true;
}

//------------------------------------------------------------------------
bool expandHourly(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback) {
    return expandTimeArray(ta, start, stop, fallback, BOH, BONH);
}

//------------------------------------------------------------------------
bool expandDaily(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback) {
    return expandTimeArray(ta, start, stop, fallback, BOD, BOND);
}

//------------------------------------------------------------------------
bool expandWeekly(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback) {
    return expandTimeArray(ta, start, stop, fallback, BOW, BONW);
}

//------------------------------------------------------------------------
bool expandMonthly(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback) {
    return expandTimeArray(ta, start, stop, fallback, BOM, BONM);
}

//------------------------------------------------------------------------
bool expandQuarterly(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback) {
    return expandTimeArray(ta, start, stop, fallback, BOQ, BONQ);
}

//------------------------------------------------------------------------
bool expandAnnually(CTimeArray& ta, const time_q& start, const time_q& stop, bool fallback) {
    return expandTimeArray(ta, start, stop, fallback, BOY, BONY);
}

//----------------------------------------------------------------------------------------------------
time_q fileLastModifyDate(const string_q& filename) {
    if (!fileExists(filename))
        return earliestDate;

    struct stat statBuf;
    stat(filename.c_str(), &statBuf);

    // Convert time_t to struct tm
    tm unused;
    tm* t = localtime_r(&statBuf.st_mtime, &unused);
    ASSERT(t);
    return time_q((uint32_t)t->tm_year + 1900, (uint32_t)t->tm_mon + 1, (uint32_t)t->tm_mday, (uint32_t)t->tm_hour,
                  (uint32_t)t->tm_min, (uint32_t)t->tm_sec);
}

//----------------------------------------------------------------------------------
typedef bool (*FILEVISITOR)(const string_q& path, void* data);
extern bool forEveryFileInFolder(const string_q& mask, FILEVISITOR func, void* data);
bool getNewestFile(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", getNewestFile, data);

    } else {
        fileInfo* r = reinterpret_cast<fileInfo*>(data);
        time_q t = fileLastModifyDate(path);
        if (t > r->fileTime) {
            r->fileTime = t;
            r->fileName = path;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------
fileInfo getNewestFileInFolder(const string_q& path) {
    fileInfo rec;
    rec.fileTime = earliestDate;
    rec.fileName = "";
    forEveryFileInFolder(path + "*", getNewestFile, &rec);
    return rec;
}

//----------------------------------------------------------------------------------
bool isSameYear(const time_q& t1, const time_q& t2) {
    return BOY(t1) == BOY(t2);
}

//----------------------------------------------------------------------------------
bool isSameMonth(const time_q& t1, const time_q& t2) {
    return BOM(t1) == BOM(t2);
}

//----------------------------------------------------------------------------------
bool isSameWeek(const time_q& t1, const time_q& t2) {
    return BOW(t1) == BOW(t2);
}

//----------------------------------------------------------------------------------
bool isSameDay(const time_q& t1, const time_q& t2) {
    return BOD(t1) == BOD(t2);
}

//----------------------------------------------------------------------------------
bool isSameHour(const time_q& t1, const time_q& t2) {
    return BOH(t1) == BOH(t2);
}

//----------------------------------------------------------------------------------
bool isSamePeriod(period_t period, blknum_t b1, blknum_t b2) {
    switch (period) {
        case BY_1:
        case BY_10:
        case BY_100:
        case BY_1000:
        case BY_10000:
        case BY_100000:
        case BY_1000000:
            if (b1 == 0 && b2 == 1)
                return false;
            return ((b1 - 1) / period) == ((b2 - 1) / period);
        case BY_YEAR:
        case BY_MONTH:
        case BY_WEEK:
        case BY_DAY:
        case BY_HOUR:
        case BY_NOTHING:
        default:
            break;
    }
    return b1 == b2;
}

//----------------------------------------------------------------------------------
bool isSamePeriod(period_t period, const time_q& t1, const time_q& t2) {
    switch (period) {
        case BY_YEAR:
            return isSameYear(t1, t2);
        case BY_MONTH:
            return isSameMonth(t1, t2);
        case BY_WEEK:
            return isSameWeek(t1, t2);
        case BY_DAY:
            return isSameDay(t1, t2);
        case BY_HOUR:
            return isSameHour(t1, t2);
        case BY_1:
        case BY_10:
        case BY_100:
        case BY_1000:
        case BY_10000:
        case BY_100000:
        case BY_1000000:
        case BY_NOTHING:
        default:
            break;
    }
    return t1 == t2;
}

//----------------------------------------------------------------------------------
string_q per_2_Str(period_t period) {
    switch (period) {
        case BY_YEAR:
            return "[{YEAR}]";
        case BY_MONTH:
            return "[{MONTH}]";
        case BY_WEEK:
            return "[{WEEK}]";
        case BY_DAY:
            return "[{DAY}]";
        case BY_HOUR:
            return "[{HOUR}]";
        case BY_1:
        case BY_10:
        case BY_100:
        case BY_1000:
        case BY_10000:
        case BY_100000:
        case BY_1000000:
            return "";
        case BY_NOTHING:
        default:
            break;
    }
    return "[{MONTH}],[{DAY}]";
}

}  // namespace qblocks
