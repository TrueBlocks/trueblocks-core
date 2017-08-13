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
#include "options.h"

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
    SFString addr;
    SFString name;
    SFString source;

public:
    CAccountName(void);
    CAccountName(const CAccountName& ac);
    ~CAccountName(void);
    CAccountName& operator=(const CAccountName& ac);

    DECLARE_NODE(CAccountName);

    // EXISTING_CODE
    explicit CAccountName(SFString& nameIn) {
        source = nameIn;
        addr = toLower(nextTokenClear(source, '\t'));
        name = nextTokenClear(source, '\t');
    }
    bool Match(const SFString& s1, const SFString& s2, const SFString& s3, bool matchCase, bool all);
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CAccountName& ac);
    bool readBackLevel(SFArchive& archive);

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

//    addr = EMPTY;
//    name = EMPTY;
//    source = EMPTY;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::Copy(const CAccountName& ac) {
    Clear();
    CBaseNode::Copy(ac);

    addr = ac.addr;
    name = ac.name;
    source = ac.source;

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
inline SFString CAccountName::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAccountNameArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CAccountNameArray);
IMPLEMENT_ARCHIVE_LIST(CAccountNameList);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern bool loadData(void);
extern uint32_t countOf(const SFString& addr);
extern SFString showName(const COptions& options);
extern SFString setName(const SFString& addr, const SFString& name);
extern CAccountNameArray accounts;
// EXISTING_CODE

