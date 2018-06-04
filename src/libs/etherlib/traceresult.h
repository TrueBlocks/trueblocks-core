#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "abilib.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CTraceResult;
typedef SFArrayBase<CTraceResult>         CTraceResultArray;
typedef SFList<CTraceResult*>             CTraceResultList;
typedef SFUniqueList<CTraceResult*>       CTraceResultListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTraceResult : public CBaseNode {
public:
    SFAddress address;
    SFString code;
    SFGas gasUsed;
    SFString output;

public:
    CTraceResult(void);
    CTraceResult(const CTraceResult& tr);
    virtual ~CTraceResult(void);
    CTraceResult& operator=(const CTraceResult& tr);

    DECLARE_NODE(CTraceResult);

    // EXISTING_CODE
    friend class CTrace;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CTraceResult& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CTraceResult& tr);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTraceResult::CTraceResult(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceResult::CTraceResult(const CTraceResult& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTraceResult::~CTraceResult(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceResult::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceResult::Init(void) {
    CBaseNode::Init();

    address = "";
    code = "";
    gasUsed = 0;
    output = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceResult::Copy(const CTraceResult& tr) {
    Clear();
    CBaseNode::Copy(tr);

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
    Copy(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CTraceResultArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CTraceResultArray);
IMPLEMENT_ARCHIVE_LIST(CTraceResultList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CTraceResult& tra);
extern SFArchive& operator>>(SFArchive& archive, CTraceResult& tra);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

