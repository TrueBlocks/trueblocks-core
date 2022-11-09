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
#include "reconciliation.h"
#include "transfer.h"

namespace qblocks {

typedef enum {
    REC_NONE = 0,
    REC_TOP = (1 << 0),
    REC_TOKENS = (1 << 1),
    REC_TRACES = (1 << 2),
    REC_SOME = (REC_TOP | REC_TOKENS),
    REC_ALL = (REC_SOME | REC_TRACES),
} recon_t;

class CStatementManager {
    struct CPreviousBalance {
      public:
        // address_t assetAddr;
        blknum_t blockNumber;
        bigint_t balance;
        CPreviousBalance& operator=(const CReconciliation& ab) {
            blockNumber = ab.blockNumber;
            balance = ab.endBal;
            return *this;
        }
        bool operator==(const CPreviousBalance& it) const {
            return ((blockNumber == it.blockNumber) && (balance == it.balance));
        }
        bool operator!=(const CPreviousBalance& it) const {
            return !operator==(it);
        }
    };
    typedef map<string, CPreviousBalance> CPreviousBalanceMap;

  public:
    CAccountName name;
    address_t accountedFor;
    blknum_t prevBlock{0};
    blknum_t nextBlock{NOPOS};
    bigint_t prevBal{0};
    recon_t which{REC_NONE};
    bool forExport{false};
    CAddressBoolMap assetFilter;
    bool getTransfers(const CTransaction& trans);
    bool getStatements(CTransaction& trans);

    CStatementManager(const address_t& aF) {
        accountedFor = aF;
    };

  private:
    CStatementManager(){};
    CPreviousBalanceMap previousBalances;
    CTransferArray transfers;
};

}  // namespace qblocks
