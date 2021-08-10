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
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "utillib.h"

namespace qblocks {

// EXISTING_CODE
class CTransaction;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CReconciliation : public CBaseNode {
  public:
    blknum_t blockNumber;
    blknum_t transactionIndex;
    timestamp_t timestamp;
    address_t assetAddr;
    string_q assetSymbol;
    uint64_t decimals;
    blknum_t prevBlk;
    bigint_t prevBlkBal;
    bigint_t begBal;
    bigint_t endBal;
    bigint_t amountIn;
    bigint_t internalIn;
    bigint_t selfDestructIn;
    bigint_t minerBaseRewardIn;
    bigint_t minerNephewRewardIn;
    bigint_t minerTxFeeIn;
    bigint_t minerUncleRewardIn;
    bigint_t prefundIn;
    bigint_t amountOut;
    bigint_t internalOut;
    bigint_t selfDestructOut;
    bigint_t gasCostOut;
    string_q reconciliationType;
    bigint_t spotPrice;

  public:
    CReconciliation(void);
    CReconciliation(const CReconciliation& re);
    virtual ~CReconciliation(void);
    CReconciliation& operator=(const CReconciliation& re);

    DECLARE_NODE(CReconciliation);

    // EXISTING_CODE
    CReconciliation(blknum_t bn, blknum_t txid, timestamp_t ts) {
        initialize();
        blockNumber = bn;
        transactionIndex = txid;
        timestamp = ts;
    }
    void initForToken(CAccountName& tokenName);
    bool reconcileEth(const CReconciliation& prevRecon, blknum_t nextBlock, const CTransaction* trans,
                      const CAccountName& accountedFor);
    bool reconcileUsingTraces(bigint_t prevEndBal, const CTransaction* trans, const CAccountName& accountedFor);
    bigint_t begBalDiff(void) const;
    bigint_t endBalCalc(void) const;
    bigint_t endBalDiff(void) const;
    bigint_t totalIn(void) const;
    bigint_t totalOut(void) const;
    bigint_t totalOutLessGas(void) const;
    bigint_t amountNet(void) const;
    bool reconciled(void) const;
    CReconciliation& operator+=(const CReconciliation& r);
    // EXISTING_CODE
    bool operator==(const CReconciliation& it) const;
    bool operator!=(const CReconciliation& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CReconciliation& v1, const CReconciliation& v2);
    friend ostream& operator<<(ostream& os, const CReconciliation& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReconciliation& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReconciliation::CReconciliation(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReconciliation::CReconciliation(const CReconciliation& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReconciliation::~CReconciliation(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReconciliation::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReconciliation::initialize(void) {
    CBaseNode::initialize();

    blockNumber = 0;
    transactionIndex = 0;
    timestamp = 0;
    assetAddr = "";
    assetSymbol = "";
    decimals = 18;
    prevBlk = 0;
    prevBlkBal = 0;
    begBal = 0;
    endBal = 0;
    amountIn = 0;
    internalIn = 0;
    selfDestructIn = 0;
    minerBaseRewardIn = 0;
    minerNephewRewardIn = 0;
    minerTxFeeIn = 0;
    minerUncleRewardIn = 0;
    prefundIn = 0;
    amountOut = 0;
    internalOut = 0;
    selfDestructOut = 0;
    gasCostOut = 0;
    reconciliationType = "";
    spotPrice = int64_t(-1);

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReconciliation::duplicate(const CReconciliation& re) {
    clear();
    CBaseNode::duplicate(re);

    blockNumber = re.blockNumber;
    transactionIndex = re.transactionIndex;
    timestamp = re.timestamp;
    assetAddr = re.assetAddr;
    assetSymbol = re.assetSymbol;
    decimals = re.decimals;
    prevBlk = re.prevBlk;
    prevBlkBal = re.prevBlkBal;
    begBal = re.begBal;
    endBal = re.endBal;
    amountIn = re.amountIn;
    internalIn = re.internalIn;
    selfDestructIn = re.selfDestructIn;
    minerBaseRewardIn = re.minerBaseRewardIn;
    minerNephewRewardIn = re.minerNephewRewardIn;
    minerTxFeeIn = re.minerTxFeeIn;
    minerUncleRewardIn = re.minerUncleRewardIn;
    prefundIn = re.prefundIn;
    amountOut = re.amountOut;
    internalOut = re.internalOut;
    selfDestructOut = re.selfDestructOut;
    gasCostOut = re.gasCostOut;
    reconciliationType = re.reconciliationType;
    spotPrice = re.spotPrice;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReconciliation& CReconciliation::operator=(const CReconciliation& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReconciliation::operator==(const CReconciliation& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return ((blockNumber == it.blockNumber) && (assetAddr == it.assetAddr));
}

//-------------------------------------------------------------------------
inline bool operator<(const CReconciliation& v1, const CReconciliation& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReconciliation> CReconciliationArray;
extern CArchive& operator>>(CArchive& archive, CReconciliationArray& array);
extern CArchive& operator<<(CArchive& archive, const CReconciliationArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CReconciliation& rec);
extern CArchive& operator>>(CArchive& archive, CReconciliation& rec);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_RECONCILIATION;

//---------------------------------------------------------------------------
// EXISTING_CODE
extern CReconciliation operator+(const CReconciliation& a, const CReconciliation& b);
typedef map<string, CReconciliation> CReconciliationMap;

extern string_q wei_2_Str(const wei_t& weiIn);
extern string_q bni_2_Str(const bigint_t& numIn);

extern string_q wei_2_Ether(const wei_t& weiIn, uint64_t decimals);
extern string_q bni_2_Ether(const bigint_t& numIn, uint64_t decimals);

extern string_q wei_2_Dollars(const timestamp_t& ts, const wei_t& weiIn, uint64_t decimals);
extern string_q bni_2_Dollars(const timestamp_t& ts, const bigint_t& numIn, uint64_t decimals);

extern string_q wei_2_Export(const blknum_t& bn, const wei_t& weiIn, uint64_t decimals);
extern string_q bni_2_Export(const timestamp_t& ts, const bigint_t& numIn, uint64_t decimals);
// EXISTING_CODE
}  // namespace qblocks
