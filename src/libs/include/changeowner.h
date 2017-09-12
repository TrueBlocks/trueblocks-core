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
class QChangeOwner;
typedef SFArrayBase<QChangeOwner>         QChangeOwnerArray;
typedef SFList<QChangeOwner*>             QChangeOwnerList;
typedef SFUniqueList<QChangeOwner*>       QChangeOwnerListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QChangeOwner : public CTransaction {
public:
    SFAddress _from;
    SFAddress _to;

public:
    QChangeOwner(void);
    QChangeOwner(const QChangeOwner& ch);
    ~QChangeOwner(void);
    QChangeOwner& operator=(const QChangeOwner& ch);

    DECLARE_NODE(QChangeOwner);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QChangeOwner& ch);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QChangeOwner::QChangeOwner(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QChangeOwner::QChangeOwner(const QChangeOwner& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QChangeOwner::~QChangeOwner(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QChangeOwner::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QChangeOwner::Init(void) {
    CTransaction::Init();

    _from = "";
    _to = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QChangeOwner::Copy(const QChangeOwner& ch) {
    Clear();
    CTransaction::Copy(ch);

    _from = ch._from;
    _to = ch._to;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QChangeOwner& QChangeOwner::operator=(const QChangeOwner& ch) {
    Copy(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QChangeOwnerArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QChangeOwnerArray);
IMPLEMENT_ARCHIVE_LIST(QChangeOwnerList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE

