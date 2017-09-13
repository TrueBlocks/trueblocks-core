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
class QTransfer;
typedef SFArrayBase<QTransfer>         QTransferArray;
typedef SFList<QTransfer*>             QTransferList;
typedef SFUniqueList<QTransfer*>       QTransferListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QTransfer : public CTransaction {
public:
    SFAddress _to;
    SFUintBN _value;

public:
    QTransfer(void);
    QTransfer(const QTransfer& tr);
    ~QTransfer(void);
    QTransfer& operator=(const QTransfer& tr);

    DECLARE_NODE(QTransfer);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QTransfer& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QTransfer& tr);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QTransfer::QTransfer(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QTransfer::QTransfer(const QTransfer& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QTransfer::~QTransfer(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransfer::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransfer::Init(void) {
    CTransaction::Init();

    _to = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransfer::Copy(const QTransfer& tr) {
    Clear();
    CTransaction::Copy(tr);

    _to = tr._to;
    _value = tr._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QTransfer& QTransfer::operator=(const QTransfer& tr) {
    Copy(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QTransferArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QTransferArray);
IMPLEMENT_ARCHIVE_LIST(QTransferList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE

