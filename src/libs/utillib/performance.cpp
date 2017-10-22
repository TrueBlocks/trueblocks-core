/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "performance.h"

namespace qblocks {

    double qbNow(void) {
        struct timeval tv;
        gettimeofday(&tv, 0);
        double secs = (double)tv.tv_sec;
        double usecs = (double)tv.tv_usec;
        return (secs + (usecs / 100000.0));
    }

}  // namespace qblocks
