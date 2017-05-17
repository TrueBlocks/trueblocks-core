#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include <sys/timeb.h>
#include <cstdint>

namespace qblocks {

    //----------------------------------------------------------------------
    class CPerformanceClock {
    protected:
        int64_t m_Seconds;
        int64_t m_uSeconds;

    public:
        CPerformanceClock(void);
        CPerformanceClock(const CPerformanceClock& t);
        CPerformanceClock(int64_t secs, int64_t usecs);

        CPerformanceClock& operator=(const CPerformanceClock& t);
        CPerformanceClock& operator=(double t);

        operator double(void) const;  // NOLINT
        bool operator>(const CPerformanceClock& q) const;
        bool operator<(const CPerformanceClock& q) const;
        bool operator>=(const CPerformanceClock& q) const;
        bool operator<=(const CPerformanceClock& q) const;
        bool operator==(const CPerformanceClock& q) const;
        bool operator!=(const CPerformanceClock& q) const;

        static CPerformanceClock Now(void);
        friend CPerformanceClock operator+(const CPerformanceClock& t1, const CPerformanceClock& t2);
        friend CPerformanceClock operator-(const CPerformanceClock& t1, const CPerformanceClock& t2);
    };

#define qbNow CPerformanceClock::Now
#define START_TIMER() double timerStart = qbNow();
#define STOP_TIMER() double timerStop = qbNow(); double timeSpent = timerStop - timerStart;

}  // namespace qblocks
