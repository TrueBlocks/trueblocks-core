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
#include "utillib.h"
#include "abi.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTraceResult : public CBaseNode {
  public:
    address_t newContract;
    string_q code;
    gas_t gasUsed;
    string_q output;

  public:
    CTraceResult(void);
    CTraceResult(const CTraceResult& tr);
    virtual ~CTraceResult(void);
    CTraceResult& operator=(const CTraceResult& tr);

    DECLARE_NODE(CTraceResult);

    // EXISTING_CODE
    friend class CTrace;
    // EXISTING_CODE
    bool operator==(const CTraceResult& it) const;
    bool operator!=(const CTraceResult& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTraceResult& v1, const CTraceResult& v2);
    friend ostream& operator<<(ostream& os, const CTraceResult& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTraceResult& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTraceResult::CTraceResult(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceResult::CTraceResult(const CTraceResult& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTraceResult::~CTraceResult(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceResult::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceResult::initialize(void) {
    CBaseNode::initialize();

    newContract = "";
    code = "";
    gasUsed = 0;
    output = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceResult::duplicate(const CTraceResult& tr) {
    clear();
    CBaseNode::duplicate(tr);

    newContract = tr.newContract;
    code = tr.code;
    gasUsed = tr.gasUsed;
    output = tr.output;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceResult& CTraceResult::operator=(const CTraceResult& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTraceResult::operator==(const CTraceResult& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return ((newContract == it.newContract) && (code == it.code) && (gasUsed == it.gasUsed) && (output == it.output));
}

//-------------------------------------------------------------------------
inline bool operator<(const CTraceResult& v1, const CTraceResult& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTraceResult> CTraceResultArray;
extern CArchive& operator>>(CArchive& archive, CTraceResultArray& array);
extern CArchive& operator<<(CArchive& archive, const CTraceResultArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CTraceResult& tra);
extern CArchive& operator>>(CArchive& archive, CTraceResult& tra);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TRACERESULT;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
