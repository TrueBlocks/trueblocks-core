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
class QRemoveOwner;
typedef SFArrayBase<QRemoveOwner>         QRemoveOwnerArray;
typedef SFList<QRemoveOwner*>             QRemoveOwnerList;
typedef SFUniqueList<QRemoveOwner*>       QRemoveOwnerListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QRemoveOwner : public CTransaction {
public:
    SFAddress _owner;

public:
    QRemoveOwner(void);
    QRemoveOwner(const QRemoveOwner& re);
    ~QRemoveOwner(void);
    QRemoveOwner& operator=(const QRemoveOwner& re);

    DECLARE_NODE(QRemoveOwner);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QRemoveOwner& re);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QRemoveOwner::QRemoveOwner(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QRemoveOwner::QRemoveOwner(const QRemoveOwner& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QRemoveOwner::~QRemoveOwner(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRemoveOwner::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRemoveOwner::Init(void) {
    CTransaction::Init();

    _owner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRemoveOwner::Copy(const QRemoveOwner& re) {
    Clear();
    CTransaction::Copy(re);

    _owner = re._owner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QRemoveOwner& QRemoveOwner::operator=(const QRemoveOwner& re) {
    Copy(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString QRemoveOwner::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QRemoveOwnerArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QRemoveOwnerArray);
IMPLEMENT_ARCHIVE_LIST(QRemoveOwnerList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

