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
class COptions : public CBlockOptions {
public:
    bool isCheck;
    bool isRaw;
    bool isCache;
    bool hashes;
    bool traces;
    bool force;
    bool normalize;
    bool silent;
    int quiet;
    SFString format;
    bool priceBlocks;

    COptions(void);
    ~COptions(void);

    SFString postProcess(const SFString& which, const SFString& str) const override;
    bool parseArguments(SFString& command) override;
    void Init(void) override;
    bool isMulti(void) const;
};

//-----------------------------------------------------------------------------
extern SFString doOneBlock(uint64_t num, const COptions& opt);
extern SFString checkOneBlock(uint64_t num, const COptions& opt);
extern SFString normalizeBlock(const SFString& inIn, bool remove, bool isByzan);
extern SFString diffStr(const SFString& str1, const SFString& str2);
extern void interumReport(ostream& os, blknum_t i);
