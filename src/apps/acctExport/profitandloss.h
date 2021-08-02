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
#include "acctlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CProfitAndLoss : public CBaseNode {
  public:
    string_q type;
    time_q lastDate;

  public:
    CProfitAndLoss(void);
    CProfitAndLoss(const CProfitAndLoss& pr);
    virtual ~CProfitAndLoss(void);
    CProfitAndLoss& operator=(const CProfitAndLoss& pr);

    DECLARE_NODE(CProfitAndLoss);

    // EXISTING_CODE
    CReconciliationMap currentStatements;
    // EXISTING_CODE
    bool operator==(const CProfitAndLoss& it) const;
    bool operator!=(const CProfitAndLoss& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CProfitAndLoss& v1, const CProfitAndLoss& v2);
    friend ostream& operator<<(ostream& os, const CProfitAndLoss& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CProfitAndLoss& pr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CProfitAndLoss::CProfitAndLoss(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CProfitAndLoss::CProfitAndLoss(const CProfitAndLoss& pr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CProfitAndLoss::~CProfitAndLoss(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProfitAndLoss::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProfitAndLoss::initialize(void) {
    CBaseNode::initialize();

    type = "";
    lastDate = earliestDate;

    // EXISTING_CODE
    currentStatements.clear();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProfitAndLoss::duplicate(const CProfitAndLoss& pr) {
    clear();
    CBaseNode::duplicate(pr);

    type = pr.type;
    lastDate = pr.lastDate;

    // EXISTING_CODE
    currentStatements = pr.currentStatements;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CProfitAndLoss& CProfitAndLoss::operator=(const CProfitAndLoss& pr) {
    duplicate(pr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CProfitAndLoss::operator==(const CProfitAndLoss& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CProfitAndLoss& v1, const CProfitAndLoss& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CProfitAndLoss> CProfitAndLossArray;
extern CArchive& operator>>(CArchive& archive, CProfitAndLossArray& array);
extern CArchive& operator<<(CArchive& archive, const CProfitAndLossArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PROFITANDLOSS;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
