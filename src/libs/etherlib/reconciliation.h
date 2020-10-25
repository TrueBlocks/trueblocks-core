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
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "abilib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CReconciliation : public CBaseNode {
  public:
    blknum_t blockNumber;
    blknum_t transactionIndex;
    timestamp_t timestamp;
    string_q asset;
    bigint_t begBal;
    bigint_t begBalDiff;
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
    bigint_t endBal;
    bigint_t endBalCalc;
    bigint_t endBalDiff;
    bigint_t amountNet;
    string_q reconciliationType;
    bool reconciled;

  public:
    CReconciliation(void);
    CReconciliation(const CReconciliation& re);
    virtual ~CReconciliation(void);
    CReconciliation& operator=(const CReconciliation& re);

    DECLARE_NODE(CReconciliation);

    // EXISTING_CODE
    bool reconcile(const CStringArray& corrections, const CReconciliation& lastStatement, blknum_t nextBlock,
                   const CTransaction* trans);
    bool reconcileUsingTraces(const CReconciliation& lastStatement, blknum_t nextBlock, const CTransaction* trans);
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
    asset = "";
    begBal = 0;
    begBalDiff = 0;
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
    endBal = 0;
    endBalCalc = 0;
    endBalDiff = 0;
    amountNet = 0;
    reconciliationType = "";
    reconciled = false;

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
    asset = re.asset;
    begBal = re.begBal;
    begBalDiff = re.begBalDiff;
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
    endBal = re.endBal;
    endBalCalc = re.endBalCalc;
    endBalDiff = re.endBalDiff;
    amountNet = re.amountNet;
    reconciliationType = re.reconciliationType;
    reconciled = re.reconciled;

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
    return ((blockNumber == it.blockNumber) && (asset == it.asset));
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
// EXISTING_CODE
}  // namespace qblocks
