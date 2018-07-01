#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include <vector>
#include <map>
#include "abilib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTraceResult : public CBaseNode {
public:
    SFAddress address;
    string_q code;
    SFGas gasUsed;
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
    bool operator==(const CTraceResult& item) const;
    bool operator!=(const CTraceResult& item) const { return !operator==(item); }
    friend bool operator<(const CTraceResult& v1, const CTraceResult& v2);
    friend ostream& operator<<(ostream& os, const CTraceResult& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTraceResult& tr);
    bool readBackLevel(SFArchive& archive) override;

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

    address = "";
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

    address = tr.address;
    code = tr.code;
    gasUsed = tr.gasUsed;
    output = tr.output;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTraceResult& CTraceResult::operator=(const CTraceResult& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTraceResult::operator==(const CTraceResult& item) const {
    // EXISTING1_CODE
    // EXISTING1_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
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
extern SFArchive& operator>>(SFArchive& archive, CTraceResultArray& array);
extern SFArchive& operator<<(SFArchive& archive, const CTraceResultArray& array);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CTraceResult& tra);
extern SFArchive& operator>>(SFArchive& archive, CTraceResult& tra);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

