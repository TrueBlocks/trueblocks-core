#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "utillib.h"

//--------------------------------------------------------------------------
class CAbiOptions : public COptionsBase {
  public:
    CAbi abi_spec;
};

//--------------------------------------------------------------------------
class CStatementOptions : public CAbiOptions {
  public:
    CLedgerManager ledgerManager{""};
};

//-----------------------------------------------------------------------------
class CBlockOptions : public CAbiOptions {
  public:
    COptionsBlockList blocks;
    CBlockOptions(void);
    string_q getBlockNumList(void);
    void Init(void);
};

//-----------------------------------------------------------------------------
class CHistoryOptions : public CBlockOptions {
  public:
    blknum_t newestBlock;
    blknum_t oldestBlock;
    CHistoryOptions(void) {
        Init();
    }
    ~CHistoryOptions(void) {
    }
    void Init(void) override {
        CBlockOptions::Init();
        newestBlock = oldestBlock = NOPOS;
    }
};

//-----------------------------------------------------------------------------
extern bool lookupDate(CBlock& block, const timestamp_t& ts, blknum_t latest);
extern bool parseBlockList2(COptionsBase* opts, COptionsBlockList& blocks, const string_q& arg, blknum_t latest);
extern bool parseTransList2(COptionsBase* opt, COptionsTransList& transList, const string_q& argIn);
extern bool parseAddressList(COptionsBase* opt, CAddressArray& addrs, const string_q& argIn);
extern bool parseTopicList2(COptionsBase* opt, CTopicArray& topics, const string_q& argIn);
extern bool parseFourbyteList(COptionsBase* opt, CFourbyteArray& fourbytes, const string_q& argIn);
extern bool parseStringList2(COptionsBase* opt, CStringArray& strings, const string_q& argIn);
extern bool getDirectionalTxId(blknum_t bn, txnum_t txid, const string_q& dir, string_q& argOut, string_q& errorMsg);
extern bool wrangleTxId(string_q& argIn, string_q& errorMsg);
