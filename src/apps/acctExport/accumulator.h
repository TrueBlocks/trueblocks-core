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
class CAccumulator : public CBaseNode {
  public:
    time_q endOfPeriod;

  public:
    CAccumulator(void);
    CAccumulator(const CAccumulator& ac);
    virtual ~CAccumulator(void);
    CAccumulator& operator=(const CAccumulator& ac);

    DECLARE_NODE(CAccumulator);

    // EXISTING_CODE
    period_t sum_type = BY_NOTHING;
    bool accumulate(const CTransaction* trans, CTransaction& summary);
    // EXISTING_CODE
    bool operator==(const CAccumulator& it) const;
    bool operator!=(const CAccumulator& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CAccumulator& v1, const CAccumulator& v2);
    friend ostream& operator<<(ostream& os, const CAccumulator& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAccumulator& ac);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccumulator::CAccumulator(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccumulator::CAccumulator(const CAccumulator& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccumulator::~CAccumulator(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccumulator::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccumulator::initialize(void) {
    CBaseNode::initialize();

    endOfPeriod = earliestDate;

    // EXISTING_CODE
    sum_type = BY_NOTHING;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccumulator::duplicate(const CAccumulator& ac) {
    clear();
    CBaseNode::duplicate(ac);

    endOfPeriod = ac.endOfPeriod;

    // EXISTING_CODE
    sum_type = ac.sum_type;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccumulator& CAccumulator::operator=(const CAccumulator& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAccumulator::operator==(const CAccumulator& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAccumulator& v1, const CAccumulator& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAccumulator> CAccumulatorArray;
extern CArchive& operator>>(CArchive& archive, CAccumulatorArray& array);
extern CArchive& operator<<(CArchive& archive, const CAccumulatorArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ACCUMULATOR;

//---------------------------------------------------------------------------
// EXISTING_CODE
extern period_t getSummaryType(const string_q& type);
// EXISTING_CODE
}  // namespace qblocks
