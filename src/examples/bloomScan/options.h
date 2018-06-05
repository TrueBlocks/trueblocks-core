#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase
{
public:
    blknum_t start;
    blknum_t stop;
    blknum_t skip;
    blknum_t nBlocks;
    bool asData;
    SFString mode;

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
inline SFString dateColor(uint64_t bn) {
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
