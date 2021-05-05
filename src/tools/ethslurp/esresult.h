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
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "utillib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CESResult : public CBaseNode {
  public:
    string_q status;
    string_q message;
    string_q result;

  public:
    CESResult(void);
    CESResult(const CESResult& es);
    virtual ~CESResult(void);
    CESResult& operator=(const CESResult& es);

    DECLARE_NODE(CESResult);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CESResult& it) const;
    bool operator!=(const CESResult& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CESResult& v1, const CESResult& v2);
    friend ostream& operator<<(ostream& os, const CESResult& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CESResult& es);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CESResult::CESResult(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CESResult::CESResult(const CESResult& es) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(es);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CESResult::~CESResult(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CESResult::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CESResult::initialize(void) {
    CBaseNode::initialize();

    status = "";
    message = "";
    result = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CESResult::duplicate(const CESResult& es) {
    clear();
    CBaseNode::duplicate(es);

    status = es.status;
    message = es.message;
    result = es.result;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CESResult& CESResult::operator=(const CESResult& es) {
    duplicate(es);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CESResult::operator==(const CESResult& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CESResult& v1, const CESResult& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CESResult> CESResultArray;
extern CArchive& operator>>(CArchive& archive, CESResultArray& array);
extern CArchive& operator<<(CArchive& archive, const CESResultArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ESRESULT;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
