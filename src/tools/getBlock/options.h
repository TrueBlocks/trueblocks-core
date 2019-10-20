#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
// BEG_CODE_DECLARE
    bool hashes_only;
    bool check;
    bool count_only;
    bool force;
    bool quiet;
    bool cache;
    bool normalize;
// END_CODE_DECLARE

    bool showZeroTrace;
    string_q format;
    bool priceBlocks;
    CBlock latest;
    uint64_t addrCounter;
    string_q filterType;
    CAddressArray filters;
    timestamp_t secsFinal;

    COptions(void);
    ~COptions(void);

    string_q postProcess(const string_q& which, const string_q& str) const override;
    bool parseArguments(string_q& command) override;
    void Init(void) override;
    bool isMulti(void) const;
};

//-----------------------------------------------------------------------------
extern string_q doOneBlock(uint64_t num, const COptions& opt);
extern string_q checkOneBlock(uint64_t num, const COptions& opt);
extern string_q getAddresses(uint64_t num, const COptions& opt);
extern string_q normalizeBlock(const string_q& inIn, bool remove, bool isByzan);
extern string_q diffStr(const string_q& str1, const string_q& str2);
extern void interumReport(ostream& os, blknum_t i);
