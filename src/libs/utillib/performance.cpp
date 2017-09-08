/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "performance.h"

namespace qblocks {

    CPerformanceClock::CPerformanceClock(void) {
        m_Seconds  = 0;
        m_uSeconds = 0;
    }

    CPerformanceClock::CPerformanceClock(const CPerformanceClock& t) {
        m_Seconds  = t.m_Seconds;
        m_uSeconds = t.m_uSeconds;
    }

    CPerformanceClock::CPerformanceClock(int64_t secs, int64_t usecs) {
        m_Seconds  = secs;
        m_uSeconds = usecs;
    }

    CPerformanceClock& CPerformanceClock::operator=(const CPerformanceClock& t) {
        m_Seconds  = t.m_Seconds;
        m_uSeconds = t.m_uSeconds;
        return *this;
    }

    CPerformanceClock& CPerformanceClock::operator=(double t) {
        m_Seconds  = (int64_t)t;  // truncate
        m_uSeconds = (int64_t)((t - m_Seconds) / 1000000.0);
        return *this;
    }

    CPerformanceClock operator+(const CPerformanceClock& t1, const CPerformanceClock& t2) {
        CPerformanceClock t;
        t.m_uSeconds = t1.m_uSeconds + t2.m_uSeconds;

        if (t.m_uSeconds >= 1000000) {
            t.m_uSeconds -= 1000000;
            t.m_Seconds = t1.m_Seconds + t2.m_Seconds + 1;
        } else {
            t.m_Seconds = t1.m_Seconds + t2.m_Seconds;
            if ((t.m_Seconds >= 1) && ((t.m_uSeconds < 0))) {
                t.m_Seconds--;
                t.m_uSeconds += 1000000;
            }
        }
        return t;
    }

    CPerformanceClock operator-(const CPerformanceClock& t1, const CPerformanceClock& t2) {
        CPerformanceClock t;
        t.m_uSeconds = t1.m_uSeconds - t2.m_uSeconds;

        if (t.m_uSeconds < 0) {
            t.m_uSeconds += 1000000;
            t.m_Seconds = t1.m_Seconds - t2.m_Seconds - 1;
        } else {
            t.m_Seconds = t1.m_Seconds - t2.m_Seconds;
        }
        return t;
    }

    CPerformanceClock::operator double(void) const {  // NOLINT
        double s1 = m_Seconds;
        double s2 = m_uSeconds;
        return (s1 + (s2 / 1000000.0));
    }

    bool CPerformanceClock::operator>(const CPerformanceClock& q) const {
        return ((m_Seconds > q.m_Seconds) ||
                ((m_Seconds == q.m_Seconds) && (m_uSeconds > q.m_uSeconds)));
    }

    bool CPerformanceClock::operator<(const CPerformanceClock& q) const {
        return ((m_Seconds < q.m_Seconds) ||
                ((m_Seconds == q.m_Seconds) && (m_uSeconds < q.m_uSeconds)));
    }

    bool CPerformanceClock::operator>=(const CPerformanceClock& q) const {
        return ((m_Seconds >= q.m_Seconds) ||
                ((m_Seconds == q.m_Seconds) && (m_uSeconds >= q.m_uSeconds)));
    }

    bool CPerformanceClock::operator<=(const CPerformanceClock& q) const {
        return ((m_Seconds <= q.m_Seconds) ||
                ((m_Seconds == q.m_Seconds) && (m_uSeconds <= q.m_uSeconds)));
    }

    bool CPerformanceClock::operator==(const CPerformanceClock& q) const {
        return ((m_Seconds == q.m_Seconds) &&
                (m_uSeconds == q.m_uSeconds));
    }

    bool CPerformanceClock::operator!=(const CPerformanceClock& q) const {
        return !operator==(q);
    }

    CPerformanceClock CPerformanceClock::Now(void) {
        struct timeval tv;
        gettimeofday(&tv, 0);
        return CPerformanceClock(tv.tv_sec, tv.tv_usec * 1000);
    }

}  // namespace qblocks
