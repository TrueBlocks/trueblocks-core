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
class QDecimals;
typedef SFArrayBase<QDecimals>         QDecimalsArray;
typedef SFList<QDecimals*>             QDecimalsList;
typedef SFUniqueList<QDecimals*>       QDecimalsListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QDecimals : public CTransaction {
public:

public:
    QDecimals(void);
    QDecimals(const QDecimals& de);
    virtual ~QDecimals(void);
    QDecimals& operator=(const QDecimals& de);

    DECLARE_NODE(QDecimals);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QDecimals& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QDecimals& de);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QDecimals::QDecimals(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QDecimals::QDecimals(const QDecimals& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QDecimals::~QDecimals(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QDecimals::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QDecimals::Init(void) {
    CTransaction::Init();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QDecimals::Copy(const QDecimals& de) {
    Clear();
    CTransaction::Copy(de);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QDecimals& QDecimals::operator=(const QDecimals& de) {
    Copy(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QDecimalsArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QDecimalsArray);
IMPLEMENT_ARCHIVE_LIST(QDecimalsList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

