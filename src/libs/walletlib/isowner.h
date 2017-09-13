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
class QIsOwner;
typedef SFArrayBase<QIsOwner>         QIsOwnerArray;
typedef SFList<QIsOwner*>             QIsOwnerList;
typedef SFUniqueList<QIsOwner*>       QIsOwnerListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QIsOwner : public CTransaction {
public:
    SFAddress _addr;

public:
    QIsOwner(void);
    QIsOwner(const QIsOwner& is);
    ~QIsOwner(void);
    QIsOwner& operator=(const QIsOwner& is);

    DECLARE_NODE(QIsOwner);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QIsOwner& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QIsOwner& is);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QIsOwner::QIsOwner(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QIsOwner::QIsOwner(const QIsOwner& is) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(is);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QIsOwner::~QIsOwner(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QIsOwner::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QIsOwner::Init(void) {
    CTransaction::Init();

    _addr = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QIsOwner::Copy(const QIsOwner& is) {
    Clear();
    CTransaction::Copy(is);

    _addr = is._addr;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QIsOwner& QIsOwner::operator=(const QIsOwner& is) {
    Copy(is);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QIsOwnerArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QIsOwnerArray);
IMPLEMENT_ARCHIVE_LIST(QIsOwnerList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE

