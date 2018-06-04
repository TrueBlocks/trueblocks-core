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
#include "utillib.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CRPCResult;
typedef SFArrayBase<CRPCResult>         CRPCResultArray;
typedef SFList<CRPCResult*>             CRPCResultList;
typedef SFUniqueList<CRPCResult*>       CRPCResultListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CRPCResult : public CBaseNode {
public:
    SFString jsonrpc;
    SFString result;
    SFString id;

public:
    CRPCResult(void);
    CRPCResult(const CRPCResult& rp);
    virtual ~CRPCResult(void);
    CRPCResult& operator=(const CRPCResult& rp);

    DECLARE_NODE(CRPCResult);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CRPCResult& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CRPCResult& rp);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRPCResult::CRPCResult(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRPCResult::CRPCResult(const CRPCResult& rp) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(rp);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRPCResult::~CRPCResult(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRPCResult::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRPCResult::Init(void) {
    CBaseNode::Init();

    jsonrpc = "";
    result = "";
    id = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRPCResult::Copy(const CRPCResult& rp) {
    Clear();
    CBaseNode::Copy(rp);

    jsonrpc = rp.jsonrpc;
    result = rp.result;
    id = rp.id;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRPCResult& CRPCResult::operator=(const CRPCResult& rp) {
    Copy(rp);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CRPCResultArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CRPCResultArray);
IMPLEMENT_ARCHIVE_LIST(CRPCResultList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

