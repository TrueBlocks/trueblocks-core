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
class QAllowance;
typedef SFArrayBase<QAllowance>         QAllowanceArray;
typedef SFList<QAllowance*>             QAllowanceList;
typedef SFUniqueList<QAllowance*>       QAllowanceListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QAllowance : public CTransaction {
public:
    SFAddress _owner;
    SFAddress _spender;

public:
    QAllowance(void);
    QAllowance(const QAllowance& al);
    virtual ~QAllowance(void);
    QAllowance& operator=(const QAllowance& al);

    DECLARE_NODE(QAllowance);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QAllowance& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QAllowance& al);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QAllowance::QAllowance(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QAllowance::QAllowance(const QAllowance& al) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(al);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QAllowance::~QAllowance(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QAllowance::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QAllowance::Init(void) {
    CTransaction::Init();

    _owner = "";
    _spender = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QAllowance::Copy(const QAllowance& al) {
    Clear();
    CTransaction::Copy(al);

    _owner = al._owner;
    _spender = al._spender;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QAllowance& QAllowance::operator=(const QAllowance& al) {
    Copy(al);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QAllowanceArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QAllowanceArray);
IMPLEMENT_ARCHIVE_LIST(QAllowanceList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

