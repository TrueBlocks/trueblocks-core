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

namespace qblocks {

// EXISTING_CODE
// Note: this class doubles as a key/value pair for json parsing
#define CKeyValue CRPCResult
#define CKeyValueArray CRPCResultArray
// EXISTING_CODE

//--------------------------------------------------------------------------
class CRPCResult : public CBaseNode {
  public:
    string_q jsonrpc;
    string_q result;
    string_q id;

  public:
    CRPCResult(void);
    CRPCResult(const CRPCResult& rp);
    virtual ~CRPCResult(void);
    CRPCResult& operator=(const CRPCResult& rp);

    DECLARE_NODE(CRPCResult);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRPCResult& it) const;
    bool operator!=(const CRPCResult& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CRPCResult& v1, const CRPCResult& v2);
    friend ostream& operator<<(ostream& os, const CRPCResult& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRPCResult& rp);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRPCResult::CRPCResult(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRPCResult::CRPCResult(const CRPCResult& rp) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(rp);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRPCResult::~CRPCResult(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRPCResult::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRPCResult::initialize(void) {
    CBaseNode::initialize();

    jsonrpc = "";
    result = "";
    id = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRPCResult::duplicate(const CRPCResult& rp) {
    clear();
    CBaseNode::duplicate(rp);

    jsonrpc = rp.jsonrpc;
    result = rp.result;
    id = rp.id;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRPCResult& CRPCResult::operator=(const CRPCResult& rp) {
    duplicate(rp);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRPCResult::operator==(const CRPCResult& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRPCResult& v1, const CRPCResult& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRPCResult> CRPCResultArray;
extern CArchive& operator>>(CArchive& archive, CRPCResultArray& array);
extern CArchive& operator<<(CArchive& archive, const CRPCResultArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_RPCRESULT;

//---------------------------------------------------------------------------
// EXISTING_CODE
extern string_q extractRPCError(const string_q& resultIn);
// EXISTING_CODE
}  // namespace qblocks
