#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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
    ~CRPCResult(void);
    CRPCResult& operator=(const CRPCResult& rp);

    DECLARE_NODE(CRPCResult);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CRPCResult& rp);
    bool readBackLevel(SFArchive& archive);

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
inline SFString CRPCResult::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CRPCResultArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CRPCResultArray);
IMPLEMENT_ARCHIVE_LIST(CRPCResultList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

