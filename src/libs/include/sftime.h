#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "sfos.h"
#include "sfdate.h"
#include "sftimeofday.h"

using timestamp_t = uint64_t;

//-------------------------------------------------------------------------
// A Date class with a granularity of 1 second
//-------------------------------------------------------------------------
class SFTime
{
public:
				SFTime          (void);
				SFTime          (const SFTime& date);

				SFTime          (uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t min, uint32_t sec);
				SFTime          (uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek, uint32_t hour, uint32_t min, uint32_t sec);
				SFTime          (uint32_t days, uint32_t hour, uint32_t min, uint32_t sec);
				SFTime          (const SFDate& date, const SFTimeOfDay& tod);
				SFTime          (const SF_TIMESTRUCT& sysTime, bool useDayOfWeek=false);
				SFTime          (const SFString& dateStr, const SFString& fmtStr);

	//	       ~SFTime          (void);

	SFTime&     operator=       (const SFTime& date);

	uint32_t    GetDay          (void) const;
	uint32_t    GetMonth        (void) const;
	uint32_t    GetYear         (void) const;
	uint32_t    GetHour         (void) const;
	uint32_t    GetMinute       (void) const;
	uint32_t    GetSecond       (void) const;
	uint32_t    GetDayOfWeek    (void) const;

	timestamp_t GetTotalSeconds (void) const;

	bool        IsValid         (void) const;

	SFTime      operator+       (const uint32_t& ts) const;
	SFTime      operator-       (const uint32_t& ts) const;
	int64_t     operator-       (SFTime& date);

	bool        operator==      (const SFTime& date) const;
	bool        operator!=      (const SFTime& date) const;

	bool        operator>       (const SFTime& date) const;
	bool        operator<       (const SFTime& date) const;

	bool        operator>=      (const SFTime& date) const;
	bool        operator<=      (const SFTime& date) const;

	bool        onTheHour       (void) const;

	SFString    Format          (uint32_t fmt) const;
	SFString    Format          (const SFString& fmt) const;

	void        fromSortStr     (const SFString& vStr);

private:
	friend void SetTime       (SFTime& date, const SFString& tStr);
	friend void SetDate       (SFTime& date, const SFString& dStr);
	friend void SetTimeAndDate(SFTime& date, const SFString& valueStr);

public:
	SFDate      getDatePart     (void) const;
	SFTimeOfDay getTimePart     (void) const;

	// Count of seconds from same epoch as SFDate uses
	long     m_nSeconds;
};

inline uint32_t SFTime::GetYear() const
{
	return getDatePart().GetYear();
}

inline uint32_t SFTime::GetMonth() const
{
	return getDatePart().GetMonth();
}

inline uint32_t SFTime::GetDay() const
{
	return getDatePart().GetDay();
}

inline uint32_t SFTime::GetHour() const
{
	return getTimePart().GetHour();
}

inline uint32_t SFTime::GetMinute() const
{
	return getTimePart().GetMinute();
}

inline uint32_t SFTime::GetSecond() const
{
	return getTimePart().GetSecond();
}

inline timestamp_t SFTime::GetTotalSeconds(void) const
{
	return m_nSeconds;
}

extern uint32_t getDayOfWeek(const SFDate& date);
inline uint32_t SFTime::GetDayOfWeek() const
{
	return ::getDayOfWeek(getDatePart());
}

// We only do the test for equality and greater than. We then use these to do all other tests
inline bool SFTime::operator==(const SFTime& date) const
{
	ASSERT(IsValid());
	return (m_nSeconds == date.m_nSeconds);
}

inline bool SFTime::operator!=(const SFTime& date) const
{
	return !operator==(date);
}

inline bool SFTime::operator>(const SFTime& date) const
{
	return (m_nSeconds > date.m_nSeconds);
}

inline bool SFTime::operator>=(const SFTime& date) const
{
	return operator>(date) || operator==(date);
}

inline bool SFTime::operator<(const SFTime& date) const
{
	return (m_nSeconds < date.m_nSeconds);
}

inline bool SFTime::operator<=(const SFTime& date) const
{
	return operator<(date) || operator==(date);
}

inline SFTime SFTime::operator+(const uint32_t& ts) const
{
	SFTime ret;
	ret.m_nSeconds = m_nSeconds + ts;
	return ret;
}

inline SFTime SFTime::operator-(const uint32_t& ts) const
{
	SFTime ret;
	ret.m_nSeconds = m_nSeconds - ts;
	return ret;
}

inline int64_t SFTime::operator-(SFTime& date)
{
	return m_nSeconds - date.m_nSeconds;
}

inline bool SFTime::IsValid() const
{
	return (m_nSeconds != (long)0xdeadbeef);
}

inline SFDate SFTime::getDatePart() const
{
	ASSERT(IsValid());
	return SFDate((m_nSeconds / SECS_PER_DAY) - 2000000000L);
}

inline SFTimeOfDay SFTime::getTimePart() const
{
	if (m_nSeconds == getDatePart().GetTotalDays())
		return SFTimeOfDay(0); // midnight at start of day

	ASSERT(IsValid());
	ASSERT(m_nSeconds >= getDatePart().GetTotalDays());

	int64_t secs = m_nSeconds;
	int64_t dateSecs = getDatePart().GetTotalDays() * SECS_PER_DAY;
	return SFTimeOfDay(uint32_t(secs-dateSecs));
}

inline bool SFTime::onTheHour(void) const
{
	return (GetMinute() < 5 || GetMinute() > 55);
}

inline SFString getPaddedDate(const SFTime& date)
{
	SFString ret = padNum2(date.GetMonth())+"_"+padNum2(date.GetDay());
	if (date.GetDayOfWeek()==1) ret += "Su";
	if (date.GetDayOfWeek()==2) ret += "M";
	if (date.GetDayOfWeek()==3) ret += "T";
	if (date.GetDayOfWeek()==4) ret += "W";
	if (date.GetDayOfWeek()==5) ret += "R";
	if (date.GetDayOfWeek()==6) ret += "F";
	if (date.GetDayOfWeek()==7) ret += "Sa";
	return ret;
}

//---------------------------------------------------------------------------------------------
extern ostream &operator <<(ostream &os, const SFTime& x);

//-----------------------------------------------------------------------------------
extern SFTime      dateFromTimeStamp(timestamp_t tsIn);
extern timestamp_t toTimeStamp(const SFTime& timeIn);
