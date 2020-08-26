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
#include "reconciliation.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CReconciliationOutput : public CBaseNode {
  public:
    blknum_t blockNumber;
    blknum_t transactionIndex;
    timestamp_t timestamp;
    string_q asset;
    string_q begBal;
    string_q begBalDiff;
    string_q weiIn;
    string_q weiOut;
    string_q internalIn;
    string_q internalOut;
    string_q selfDestructIn;
    string_q selfDestructOut;
    string_q minerBaseRewardIn;
    string_q minerNephewRewardIn;
    string_q minerTxFeeIn;
    string_q minerUncleRewardIn;
    string_q prefundIn;
    string_q gasCostOut;
    string_q endBal;
    string_q endBalCalc;
    string_q endBalDiff;
    string_q reconciliationType;
    bool reconciled;

  public:
    CReconciliationOutput(void);
    CReconciliationOutput(const CReconciliationOutput& re);
    virtual ~CReconciliationOutput(void);
    CReconciliationOutput& operator=(const CReconciliationOutput& re);

    DECLARE_NODE(CReconciliationOutput);

    // EXISTING_CODE
    explicit CReconciliationOutput(const CReconciliation& nums);
    CReconciliation nums;
    // EXISTING_CODE
    bool operator==(const CReconciliationOutput& item) const;
    bool operator!=(const CReconciliationOutput& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CReconciliationOutput& v1, const CReconciliationOutput& v2);
    friend ostream& operator<<(ostream& os, const CReconciliationOutput& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReconciliationOutput& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReconciliationOutput::CReconciliationOutput(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReconciliationOutput::CReconciliationOutput(const CReconciliationOutput& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReconciliationOutput::~CReconciliationOutput(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReconciliationOutput::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReconciliationOutput::initialize(void) {
    CBaseNode::initialize();

    blockNumber = 0;
    transactionIndex = 0;
    timestamp = 0;
    asset = "";
    begBal = "";
    begBalDiff = "";
    weiIn = "";
    weiOut = "";
    internalIn = "";
    internalOut = "";
    selfDestructIn = "";
    selfDestructOut = "";
    minerBaseRewardIn = "";
    minerNephewRewardIn = "";
    minerTxFeeIn = "";
    minerUncleRewardIn = "";
    prefundIn = "";
    gasCostOut = "";
    endBal = "";
    endBalCalc = "";
    endBalDiff = "";
    reconciliationType = "";
    reconciled = false;

    // EXISTING_CODE
    // nums = CReconciliation();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReconciliationOutput::duplicate(const CReconciliationOutput& re) {
    clear();
    CBaseNode::duplicate(re);

    blockNumber = re.blockNumber;
    transactionIndex = re.transactionIndex;
    timestamp = re.timestamp;
    asset = re.asset;
    begBal = re.begBal;
    begBalDiff = re.begBalDiff;
    weiIn = re.weiIn;
    weiOut = re.weiOut;
    internalIn = re.internalIn;
    internalOut = re.internalOut;
    selfDestructIn = re.selfDestructIn;
    selfDestructOut = re.selfDestructOut;
    minerBaseRewardIn = re.minerBaseRewardIn;
    minerNephewRewardIn = re.minerNephewRewardIn;
    minerTxFeeIn = re.minerTxFeeIn;
    minerUncleRewardIn = re.minerUncleRewardIn;
    prefundIn = re.prefundIn;
    gasCostOut = re.gasCostOut;
    endBal = re.endBal;
    endBalCalc = re.endBalCalc;
    endBalDiff = re.endBalDiff;
    reconciliationType = re.reconciliationType;
    reconciled = re.reconciled;

    // EXISTING_CODE
    nums = re.nums;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReconciliationOutput& CReconciliationOutput::operator=(const CReconciliationOutput& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReconciliationOutput::operator==(const CReconciliationOutput& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CReconciliationOutput& v1, const CReconciliationOutput& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReconciliationOutput> CReconciliationOutputArray;
extern CArchive& operator>>(CArchive& archive, CReconciliationOutputArray& array);
extern CArchive& operator<<(CArchive& archive, const CReconciliationOutputArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CReconciliationOutput& rec);
extern CArchive& operator>>(CArchive& archive, CReconciliationOutput& rec);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_RECONCILIATIONOUTPUT;

//---------------------------------------------------------------------------
// EXISTING_CODE
//------------------------------------------------------------
extern string_q bni_2_Ether(const bigint_t& num);
extern string_q bni_2_Dollars(const timestamp_t& ts, const bigint_t& num);
// EXISTING_CODE
}  // namespace qblocks
