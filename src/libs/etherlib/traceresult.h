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
#include "ethtypes.h"
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
    SFUint32 gasUsed;
    SFString output;

public:
    CTraceResult(void);
    CTraceResult(const CTraceResult& tr);
    ~CTraceResult(void);
    CTraceResult& operator=(const CTraceResult& tr);

    DECLARE_NODE(CTraceResult);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CTraceResult& tr);
    bool readBackLevel(SFArchive& archive);

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

    gasUsed = 0;
//    output = EMPTY;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceResult::Copy(const CTraceResult& tr) {
    Clear();
    CBaseNode::Copy(tr);

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
inline SFString CTraceResult::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
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

