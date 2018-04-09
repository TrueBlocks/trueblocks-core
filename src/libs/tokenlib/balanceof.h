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
class QBalanceOf;
typedef SFArrayBase<QBalanceOf>         QBalanceOfArray;
typedef SFList<QBalanceOf*>             QBalanceOfList;
typedef SFUniqueList<QBalanceOf*>       QBalanceOfListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QBalanceOf : public CTransaction {
public:
    SFAddress _owner;

public:
    QBalanceOf(void);
    QBalanceOf(const QBalanceOf& ba);
    virtual ~QBalanceOf(void);
    QBalanceOf& operator=(const QBalanceOf& ba);

    DECLARE_NODE(QBalanceOf);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QBalanceOf& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QBalanceOf& ba);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QBalanceOf::QBalanceOf(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QBalanceOf::QBalanceOf(const QBalanceOf& ba) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ba);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QBalanceOf::~QBalanceOf(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QBalanceOf::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QBalanceOf::Init(void) {
    CTransaction::Init();

    _owner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QBalanceOf::Copy(const QBalanceOf& ba) {
    Clear();
    CTransaction::Copy(ba);

    _owner = ba._owner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QBalanceOf& QBalanceOf::operator=(const QBalanceOf& ba) {
    Copy(ba);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QBalanceOfArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QBalanceOfArray);
IMPLEMENT_ARCHIVE_LIST(QBalanceOfList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

