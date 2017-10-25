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

    gasUsed = 0;
    output = "";

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

