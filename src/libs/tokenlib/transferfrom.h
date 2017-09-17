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
class QTransferFrom;
typedef SFArrayBase<QTransferFrom>         QTransferFromArray;
typedef SFList<QTransferFrom*>             QTransferFromList;
typedef SFUniqueList<QTransferFrom*>       QTransferFromListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QTransferFrom : public CTransaction {
public:
    SFAddress _from;
    SFAddress _to;
    SFUintBN _value;

public:
    QTransferFrom(void);
    QTransferFrom(const QTransferFrom& tr);
    virtual ~QTransferFrom(void);
    QTransferFrom& operator=(const QTransferFrom& tr);

    DECLARE_NODE(QTransferFrom);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QTransferFrom& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QTransferFrom& tr);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QTransferFrom::QTransferFrom(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QTransferFrom::QTransferFrom(const QTransferFrom& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QTransferFrom::~QTransferFrom(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferFrom::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferFrom::Init(void) {
    CTransaction::Init();

    _from = "";
    _to = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferFrom::Copy(const QTransferFrom& tr) {
    Clear();
    CTransaction::Copy(tr);

    _from = tr._from;
    _to = tr._to;
    _value = tr._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QTransferFrom& QTransferFrom::operator=(const QTransferFrom& tr) {
    Copy(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QTransferFromArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QTransferFromArray);
IMPLEMENT_ARCHIVE_LIST(QTransferFromList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

