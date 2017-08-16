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
class QAddOwner;
typedef SFArrayBase<QAddOwner>         QAddOwnerArray;
typedef SFList<QAddOwner*>             QAddOwnerList;
typedef SFUniqueList<QAddOwner*>       QAddOwnerListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QAddOwner : public CTransaction {
public:
    SFAddress _owner;

public:
    QAddOwner(void);
    QAddOwner(const QAddOwner& ad);
    ~QAddOwner(void);
    QAddOwner& operator=(const QAddOwner& ad);

    DECLARE_NODE(QAddOwner);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QAddOwner& ad);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QAddOwner::QAddOwner(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QAddOwner::QAddOwner(const QAddOwner& ad) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ad);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QAddOwner::~QAddOwner(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QAddOwner::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QAddOwner::Init(void) {
    CTransaction::Init();

    _owner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QAddOwner::Copy(const QAddOwner& ad) {
    Clear();
    CTransaction::Copy(ad);

    _owner = ad._owner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QAddOwner& QAddOwner::operator=(const QAddOwner& ad) {
    Copy(ad);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString QAddOwner::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QAddOwnerArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QAddOwnerArray);
IMPLEMENT_ARCHIVE_LIST(QAddOwnerList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

