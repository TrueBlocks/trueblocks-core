//
//  traceCheck.h
//  proj
//
//  Created by Thomas Jay Rush on 10/14/17.
//  Copyright © 2017 com.GHC. All rights reserved.
//

#ifndef traceCheck_h
#define traceCheck_h

#include "etherlib.h"
#include "options.h"
inline void queryBlock(CBlock& block, blknum_t bn) { queryBlock(block, asStringU(bn), true, false); verbose = true; }
inline void queryTraces(CTraceArray& traces, const CBlock& block, uint32_t txID) {
    etherlib_init();
    getTraces(traces, block.transactions[txID].hash);
}

#endif /* traceCheck_h */
