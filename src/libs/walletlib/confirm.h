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
class QConfirm;
typedef SFArrayBase<QConfirm>         QConfirmArray;
typedef SFList<QConfirm*>             QConfirmList;
typedef SFUniqueList<QConfirm*>       QConfirmListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QConfirm : public CTransaction {
public:
    SFString _h;

public:
    QConfirm(void);
    QConfirm(const QConfirm& co);
    ~QConfirm(void);
    QConfirm& operator=(const QConfirm& co);

    DECLARE_NODE(QConfirm);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QConfirm& co);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QConfirm::QConfirm(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QConfirm::QConfirm(const QConfirm& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QConfirm::~QConfirm(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirm::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirm::Init(void) {
    CTransaction::Init();

    _h = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirm::Copy(const QConfirm& co) {
    Clear();
    CTransaction::Copy(co);

    _h = co._h;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QConfirm& QConfirm::operator=(const QConfirm& co) {
    Copy(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QConfirmArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QConfirmArray);
IMPLEMENT_ARCHIVE_LIST(QConfirmList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE

