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
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions;
class CSourceSearch {
  public:
    COptions* opt;
    uint64_t depth{0};
    static bool pet_names;
    static uint64_t max_depth;
    static uint64_t max_records;
    static blknum_t firstBlock;
    static CAccountNameMap whiteList;
    static CAddressBoolMap purpleList;
    static bool initialized;
    static bool debugging;
    CSourceSearch(COptions* o);
    bool isPurpleList(const address_t& addr) const {
        return purpleList.size() == 0 || purpleList[addr];
    }
    bool isWhiteListed(CTransfer* tt) const {
        if (whiteList.size() == 0) {
            return true;
        }
        CAccountName name = whiteList[tt->assetAddr];
        if (name.address.empty()) {
            return false;
        }
        tt->assetSymbol = name.symbol;
        return true;
    }
    bool traceTransfer(CTransfer& transfer, const string_q& id);
    void showTransfer(const string_q& msg, const CTransfer* transfer) const;
    bool parseAppearances(CAppearanceArray& array, const string_q& cmd);
    void getSenderTransactions(CTransactionArray& senderTxs, const address_t& sender, blknum_t lastBlock);
    bool getTransactionAndCache(const address_t& sender, CTransaction& trans, const CAppearanceArray& apps,
                                size_t which);
    string_q transfer_2_Id(const CTransfer* transfer) const {
        return transfer->assetAddr + "_" + uint_2_Str(transfer->blockNumber) + "." +
               uint_2_Str(transfer->transactionIndex) + "." + uint_2_Str(transfer->logIndex);
    }
    address_t coloredName(const address_t& addr) const;
    string_q addr_2_Name(const address_t& addr) const;
};

//-----------------------------------------------------------------------------
extern string_q getJigger(const string_q& msg, uint64_t depth);
extern string_q addr_2_Color(const address_t& addr);
extern bool writeToCache(CTransaction& trans);
extern void showFromCache(const string_q& id);
