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
class QTotalSupply;
typedef SFArrayBase<QTotalSupply>         QTotalSupplyArray;
typedef SFList<QTotalSupply*>             QTotalSupplyList;
typedef SFUniqueList<QTotalSupply*>       QTotalSupplyListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QTotalSupply : public CTransaction {
public:

public:
    QTotalSupply(void);
    QTotalSupply(const QTotalSupply& to);
    virtual ~QTotalSupply(void);
    QTotalSupply& operator=(const QTotalSupply& to);

    DECLARE_NODE(QTotalSupply);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QTotalSupply& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QTotalSupply& to);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QTotalSupply::QTotalSupply(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QTotalSupply::QTotalSupply(const QTotalSupply& to) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(to);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QTotalSupply::~QTotalSupply(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTotalSupply::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTotalSupply::Init(void) {
    CTransaction::Init();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTotalSupply::Copy(const QTotalSupply& to) {
    Clear();
    CTransaction::Copy(to);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QTotalSupply& QTotalSupply::operator=(const QTotalSupply& to) {
    Copy(to);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QTotalSupplyArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QTotalSupplyArray);
IMPLEMENT_ARCHIVE_LIST(QTotalSupplyList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

