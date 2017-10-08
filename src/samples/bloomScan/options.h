#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase
{
public:
    blknum_t start;
    blknum_t stop;
    blknum_t skip;
    blknum_t nBlocks;
    bool     display;

	        COptions  (void);
	       ~COptions  (void);

	bool parseArguments(SFString& command);
	bool parseArguments(int argc, const char *argv[]);
	void Init (void);
};

//-----------------------------------------------------------------------------
extern bool visitNonEmptyBlock(CBlock& node, void *data);
extern bool displayBloom(CBlock& node, void *data);
extern bool checkBloom(CBlock& node, void *data);
inline SFString dateColor(SFUint32 bn) {
	if (1442339 <= bn && bn <= 1439214) return bBlue;
	return bYellow;
}
inline SFTime toDate(timestamp_t ts) { return dateFromTimeStamp(ts); }
#define null ((void*)0)
#define toSpaces dbgBloom

inline bool COptions::parseArguments(int argc, const char *argv[]) {
    prepareArguments(argc, argv);
    parseArguments(commandList);
    nBlocks = stop - start;
    return true;
}
