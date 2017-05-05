#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include <sys/timeb.h>

//----------------------------------------------------------------------
class CPerformanceClock
{
protected:
	long m_Seconds;
	long m_uSeconds;

public:
	CPerformanceClock(void)
		{
			m_Seconds  = 0;
			m_uSeconds = 0;
		}

	CPerformanceClock(const CPerformanceClock& t)
		{
			m_Seconds  = t.m_Seconds;
			m_uSeconds = t.m_uSeconds;
		}

	CPerformanceClock(long secs, long usecs)
		{
			m_Seconds  = secs;
			m_uSeconds = usecs;
		}

	CPerformanceClock& operator=(const CPerformanceClock& t)
		{
			m_Seconds  = t.m_Seconds;
			m_uSeconds = t.m_uSeconds;
			return *this;
		}

	CPerformanceClock& operator=(double t)
		{
			m_Seconds  = (long)t; // truncate
			m_uSeconds = (long)((t - m_Seconds) / 1000000.0);
			return *this;
		}

	friend CPerformanceClock operator+(const CPerformanceClock& t1, const CPerformanceClock& t2)
	{
		CPerformanceClock t;
		t.m_uSeconds = t1.m_uSeconds + t2.m_uSeconds;

		if(t.m_uSeconds >= 1000000)
		{
			t.m_uSeconds -= 1000000;
			t.m_Seconds = t1.m_Seconds + t2.m_Seconds + 1;
		} else
		{
			t.m_Seconds = t1.m_Seconds + t2.m_Seconds;
			if((t.m_Seconds >= 1) && ((t.m_uSeconds < 0)))
			{
				t.m_Seconds--;
				t.m_uSeconds += 1000000;
			}
		}
		return t;
	}

	friend CPerformanceClock operator-(const CPerformanceClock& t1, const CPerformanceClock& t2)
	{
		CPerformanceClock t;
		t.m_uSeconds = t1.m_uSeconds - t2.m_uSeconds;

		if(t.m_uSeconds < 0)
		{
			t.m_uSeconds += 1000000;
			t.m_Seconds = t1.m_Seconds - t2.m_Seconds - 1;
		} else
		{
			 t.m_Seconds = t1.m_Seconds - t2.m_Seconds;
		}
		return t;
	}

	operator double(void) const
	{
		return ((double)m_Seconds + ((double)m_uSeconds / 1000000.0));
	}

	bool operator>(const CPerformanceClock& q) const
	{
		return ((m_Seconds > q.m_Seconds) ||
						((m_Seconds == q.m_Seconds) && (m_uSeconds > q.m_uSeconds)));
	}

	bool operator<(const CPerformanceClock& q) const
	{
		return ((m_Seconds < q.m_Seconds) ||
						((m_Seconds == q.m_Seconds) && (m_uSeconds < q.m_uSeconds)));
	}

	bool operator>=(const CPerformanceClock& q) const
		{
			return ((m_Seconds >= q.m_Seconds) ||
							((m_Seconds == q.m_Seconds) && (m_uSeconds >= q.m_uSeconds)));
		}

	bool operator<=(const CPerformanceClock& q) const
		{
			return ((m_Seconds <= q.m_Seconds) ||
							((m_Seconds == q.m_Seconds) && (m_uSeconds <= q.m_uSeconds)));
		}

	bool operator==(const CPerformanceClock& q)
		{
			return ((m_Seconds == q.m_Seconds) &&
							(m_uSeconds == q.m_uSeconds));
		}

	bool operator!=(const CPerformanceClock& q)
		{
			return !operator==(q);
		}

	static CPerformanceClock Now(void)
		{
			struct timeb _t;
			ftime(&_t);
			return CPerformanceClock((long)_t.time, _t.millitm * 1000);
		}
};

#define vrNow CPerformanceClock::Now
#define PERF_START()	double timerStart = vrNow();
#define PERF_STOP()	double timerStop = vrNow(); double timeSpent = timerStop - timerStart;
