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
    blknum_t bn;
    timestamp_t ts;
    string_q asset;
    bigint_t begBal;
    bigint_t begBalDiff;
    bigint_t inflow;
    bigint_t intInflow;
    bigint_t selfDestructInflow;
    bigint_t miningInflow;
    bigint_t prefundInflow;
    bigint_t outflow;
    bigint_t intOutflow;
    bigint_t selfDestructOutflow;
    bigint_t gasCostOutflow;
    bigint_t endBal;
    bigint_t endBalCalc;
    bigint_t endBalDiff;
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
    bool operator==(const CReconciliation& item) const;
    bool operator!=(const CReconciliation& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CReconciliation& v1, const CReconciliation& v2);
    friend ostream& operator<<(ostream& os, const CReconciliation& item);

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

    bn = 0;
    ts = 0;
    asset = "";
    begBal = 0;
    begBalDiff = 0;
    inflow = 0;
    intInflow = 0;
    selfDestructInflow = 0;
    miningInflow = 0;
    prefundInflow = 0;
    outflow = 0;
    intOutflow = 0;
    selfDestructOutflow = 0;
    gasCostOutflow = 0;
    endBal = 0;
    endBalCalc = 0;
    endBalDiff = 0;
    reconciliationType = "";
    reconciled = false;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReconciliation::duplicate(const CReconciliation& re) {
    clear();
    CBaseNode::duplicate(re);

    bn = re.bn;
    ts = re.ts;
    asset = re.asset;
    begBal = re.begBal;
    begBalDiff = re.begBalDiff;
    inflow = re.inflow;
    intInflow = re.intInflow;
    selfDestructInflow = re.selfDestructInflow;
    miningInflow = re.miningInflow;
    prefundInflow = re.prefundInflow;
    outflow = re.outflow;
    intOutflow = re.intOutflow;
    selfDestructOutflow = re.selfDestructOutflow;
    gasCostOutflow = re.gasCostOutflow;
    endBal = re.endBal;
    endBalCalc = re.endBalCalc;
    endBalDiff = re.endBalDiff;
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
inline bool CReconciliation::operator==(const CReconciliation& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
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
// EXISTING_CODE
}  // namespace qblocks
