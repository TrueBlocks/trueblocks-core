#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "sfdate.h"

class SFTime;
class SFTimeOfDay;

//----------------------------------------------------------------------------
// A Time of Day class with a granularity of 1 second which
// is used in conjunction with the time class SFDate in the SFTime class
//----------------------------------------------------------------------------
class SFTimeOfDay
{
public:
				  SFTimeOfDay      (void);
				  SFTimeOfDay      (const SFTimeOfDay& tod);

private:
// Everything is private because on the SFTime class (which is exposed to end users and
// is a 'friend' can use this class
				  SFTimeOfDay      (uint32_t h, uint32_t m, uint32_t s);
				  SFTimeOfDay      (const SF_TIMESTRUCT& sysTime);
				  SFTimeOfDay      (uint32_t secs);
				  SFTimeOfDay      (const SFString& dateStr, const SFString& fmtStr);

//			     ~SFTimeOfDay      (void);

	SFTimeOfDay&  operator=        (const SFTimeOfDay& tod);

	uint32_t      GetHour          (void) const;
	uint32_t      GetMinute        (void) const;
	uint32_t      GetSecond        (void) const;
	uint32_t      GetTotalSeconds  (void) const;

	bool          operator==       (const SFTimeOfDay& tod) const;
	bool          operator!=       (const SFTimeOfDay& tod) const;

	bool          operator>        (const SFTimeOfDay& tod) const;
	bool          operator<        (const SFTimeOfDay& tod) const;

	bool          operator>=       (const SFTimeOfDay& tod) const;
	bool          operator<=       (const SFTimeOfDay& tod) const;

	SFString      Format           (const SFString& fmt="") const;

	bool          IsValid          (void) const;

private:
	uint32_t m_nSeconds;

	friend class SFTime;
	friend void  SetTime    (SFTime& date, const SFString& tStr);
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline uint32_t SFTimeOfDay::GetHour() const
{
	ASSERT(IsValid());
	return (uint32_t)(m_nSeconds / SECS_PER_HOUR);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline uint32_t SFTimeOfDay::GetMinute() const
{
	ASSERT(IsValid());
	uint32_t secsInHours = GetHour() * SECS_PER_HOUR;
	return (uint32_t)((m_nSeconds - secsInHours) / SECS_PER_MIN);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline uint32_t SFTimeOfDay::GetSecond() const
{
	ASSERT(IsValid());
	uint32_t secsInHours = GetHour()   * SECS_PER_HOUR;
	uint32_t secsInMins  = GetMinute() * SECS_PER_MIN;
	return (m_nSeconds - secsInHours - secsInMins);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline uint32_t SFTimeOfDay::GetTotalSeconds() const
{
	ASSERT(IsValid());
	return m_nSeconds;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool SFTimeOfDay::IsValid() const
{
	return (m_nSeconds != SECS_PER_DAY);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool SFTimeOfDay::operator==(const SFTimeOfDay& tod) const
{
	return (m_nSeconds == tod.m_nSeconds);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool SFTimeOfDay::operator!=(const SFTimeOfDay& tod) const
{
	return !operator==(tod);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool SFTimeOfDay::operator>(const SFTimeOfDay& tod) const
{
	return (m_nSeconds > tod.m_nSeconds);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool SFTimeOfDay::operator<(const SFTimeOfDay& tod) const
{
	return (m_nSeconds < tod.m_nSeconds);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool SFTimeOfDay::operator>=(const SFTimeOfDay& tod) const
{
	return (m_nSeconds >= tod.m_nSeconds);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool SFTimeOfDay::operator<=(const SFTimeOfDay& tod) const
{
	return (m_nSeconds <= tod.m_nSeconds);
}
