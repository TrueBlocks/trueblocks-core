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
#include "transaction.h"

//--------------------------------------------------------------------------
class QName;
typedef SFArrayBase<QName>         QNameArray;
typedef SFList<QName*>             QNameList;
typedef SFUniqueList<QName*>       QNameListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QName : public CTransaction {
public:

public:
    QName(void);
    QName(const QName& na);
    virtual ~QName(void);
    QName& operator=(const QName& na);

    DECLARE_NODE(QName);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QName& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QName& na);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QName::QName(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QName::QName(const QName& na) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(na);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QName::~QName(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QName::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QName::Init(void) {
    CTransaction::Init();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QName::Copy(const QName& na) {
    Clear();
    CTransaction::Copy(na);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QName& QName::operator=(const QName& na) {
    Copy(na);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QNameArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QNameArray);
IMPLEMENT_ARCHIVE_LIST(QNameList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

