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
class CReconciliationNumeric : public CBaseNode {
  public:
    blknum_t blockNum;
    string_q asset;
    bigint_t begBal;
    bigint_t begBalDiff;
    bigint_t inflow;
    bigint_t outflow;
    bigint_t intInflow;
    bigint_t intOutflow;
    bigint_t suicideInflow;
    bigint_t suicideOutflow;
    bigint_t miningInflow;
    bigint_t prefundInflow;
    bigint_t weiGasCost;
    bigint_t endBal;
    bigint_t endBalCalc;
    bigint_t endBalDiff;
    string_q reconciliationType;
    bool reconciled;

  public:
    CReconciliationNumeric(void);
    CReconciliationNumeric(const CReconciliationNumeric& re);
    virtual ~CReconciliationNumeric(void);
    CReconciliationNumeric& operator=(const CReconciliationNumeric& re);

    DECLARE_NODE(CReconciliationNumeric);

    // EXISTING_CODE
    bool reconcile(const CStringArray& corrections, const CReconciliationNumeric& lastStatement,
                   const address_t& forAddr, blknum_t nextBlock, const CTransaction* trans);
    bool reconcileUsingTraces(const CReconciliationNumeric& lastStatement, const address_t& forAddr, blknum_t nextBlock,
                              const CTransaction* trans);
    // EXISTING_CODE
    bool operator==(const CReconciliationNumeric& item) const;
    bool operator!=(const CReconciliationNumeric& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CReconciliationNumeric& v1, const CReconciliationNumeric& v2);
    friend ostream& operator<<(ostream& os, const CReconciliationNumeric& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReconciliationNumeric& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReconciliationNumeric::CReconciliationNumeric(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReconciliationNumeric::CReconciliationNumeric(const CReconciliationNumeric& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReconciliationNumeric::~CReconciliationNumeric(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReconciliationNumeric::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReconciliationNumeric::initialize(void) {
    CBaseNode::initialize();

    blockNum = 0;
    asset = "";
    begBal = 0;
    begBalDiff = 0;
    inflow = 0;
    outflow = 0;
    intInflow = 0;
    intOutflow = 0;
    suicideInflow = 0;
    suicideOutflow = 0;
    miningInflow = 0;
    prefundInflow = 0;
    weiGasCost = 0;
    endBal = 0;
    endBalCalc = 0;
    endBalDiff = 0;
    reconciliationType = "";
    reconciled = false;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReconciliationNumeric::duplicate(const CReconciliationNumeric& re) {
    clear();
    CBaseNode::duplicate(re);

    blockNum = re.blockNum;
    asset = re.asset;
    begBal = re.begBal;
    begBalDiff = re.begBalDiff;
    inflow = re.inflow;
    outflow = re.outflow;
    intInflow = re.intInflow;
    intOutflow = re.intOutflow;
    suicideInflow = re.suicideInflow;
    suicideOutflow = re.suicideOutflow;
    miningInflow = re.miningInflow;
    prefundInflow = re.prefundInflow;
    weiGasCost = re.weiGasCost;
    endBal = re.endBal;
    endBalCalc = re.endBalCalc;
    endBalDiff = re.endBalDiff;
    reconciliationType = re.reconciliationType;
    reconciled = re.reconciled;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReconciliationNumeric& CReconciliationNumeric::operator=(const CReconciliationNumeric& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReconciliationNumeric::operator==(const CReconciliationNumeric& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CReconciliationNumeric& v1, const CReconciliationNumeric& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReconciliationNumeric> CReconciliationNumericArray;
extern CArchive& operator>>(CArchive& archive, CReconciliationNumericArray& array);
extern CArchive& operator<<(CArchive& archive, const CReconciliationNumericArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CReconciliationNumeric& rec);
extern CArchive& operator>>(CArchive& archive, CReconciliationNumeric& rec);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_RECONCILIATIONNUMERIC;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
