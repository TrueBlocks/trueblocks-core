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
class CNameValue;
typedef SFArrayBase<CNameValue>         CNameValueArray;
typedef SFList<CNameValue*>             CNameValueList;
typedef SFUniqueList<CNameValue*>       CNameValueListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CNameValue : public CBaseNode {
public:
    SFString name;
    SFString value;

public:
    CNameValue(void);
    CNameValue(const CNameValue& na);
    ~CNameValue(void);
    CNameValue& operator=(const CNameValue& na);

    DECLARE_NODE(CNameValue);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CNameValue& na);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNameValue::CNameValue(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNameValue::CNameValue(const CNameValue& na) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(na);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNameValue::~CNameValue(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNameValue::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNameValue::Init(void) {
    CBaseNode::Init();

    name = "";
    value = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNameValue::Copy(const CNameValue& na) {
    Clear();
    CBaseNode::Copy(na);

    name = na.name;
    value = na.value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNameValue& CNameValue::operator=(const CNameValue& na) {
    Copy(na);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString CNameValue::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CNameValueArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CNameValueArray);
IMPLEMENT_ARCHIVE_LIST(CNameValueList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

