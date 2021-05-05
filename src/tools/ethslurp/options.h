#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include <algorithm>
#include "etherlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

class COptions : public CBlockOptions {
  public:
    // BEG_CODE_DECLARE
    CAddressArray addrs;
    bool appearances;
    // END_CODE_DECLARE

    CStringArray typesList;
    string_q header;
    string_q displayString;
    string_q exportFormat;
    string_q formatString;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool getFormatString(const string_q& name, bool ignoreBlank, string_q& fmtOut);
    bool buildDisplayStrings(void);
};

//--------------------------------------------------------------------------------
inline string_q toEtherscan(const string_q& type) {
    if (type == "token")
        return "tokentx";
    else if (type == "nfts")
        return "tokennfttx";
    else if (type == "miner")
        return "getminedblocks&blocktype=blocks";
    else if (type == "int")
        return "txlistinternal";
    return "txlist";
}

//-------------------------------------------------------------------------
inline bool sortByBlockNumTxId(const CTransaction& v1, const CTransaction& v2) {
    if (v1.blockNumber != v2.blockNumber)
        return v1.blockNumber < v2.blockNumber;
    else if (v1.transactionIndex != v2.transactionIndex)
        return v1.transactionIndex < v2.transactionIndex;
    return false;
}
