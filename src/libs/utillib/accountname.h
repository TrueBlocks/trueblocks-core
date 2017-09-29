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
#include "basetypes.h"
#include "basenode.h"
#include "sfarchive.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CAccountName;
typedef SFArrayBase<CAccountName>         CAccountNameArray;
typedef SFList<CAccountName*>             CAccountNameList;
typedef SFUniqueList<CAccountName*>       CAccountNameListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccountName : public CBaseNode {
public:
    SFString symbol;
    SFString name;
    SFString addr;
    SFString source;
    SFString description;

public:
    CAccountName(void);
    CAccountName(const CAccountName& ac);
    virtual ~CAccountName(void);
    CAccountName& operator=(const CAccountName& ac);

    DECLARE_NODE(CAccountName);

    // EXISTING_CODE
    explicit CAccountName(SFString& nameIn);
    bool Match(const SFString& s1, const SFString& s2, const SFString& s3, bool matchCase, bool all);
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CAccountName& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CAccountName& ac);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccountName::CAccountName(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccountName::CAccountName(const CAccountName& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccountName::~CAccountName(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::Init(void) {
    CBaseNode::Init();

    symbol = "";
    name = "";
    addr = "";
    source = "";
    description = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::Copy(const CAccountName& ac) {
    Clear();
    CBaseNode::Copy(ac);

    symbol = ac.symbol;
    name = ac.name;
    addr = ac.addr;
    source = ac.source;
    description = ac.description;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAccountName& CAccountName::operator=(const CAccountName& ac) {
    Copy(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAccountNameArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CAccountNameArray);
IMPLEMENT_ARCHIVE_LIST(CAccountNameList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

