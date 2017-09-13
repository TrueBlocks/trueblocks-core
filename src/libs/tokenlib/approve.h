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
class QApprove;
typedef SFArrayBase<QApprove>         QApproveArray;
typedef SFList<QApprove*>             QApproveList;
typedef SFUniqueList<QApprove*>       QApproveListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QApprove : public CTransaction {
public:
    SFAddress _spender;
    SFUintBN _value;

public:
    QApprove(void);
    QApprove(const QApprove& ap);
    ~QApprove(void);
    QApprove& operator=(const QApprove& ap);

    DECLARE_NODE(QApprove);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QApprove& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QApprove& ap);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QApprove::QApprove(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QApprove::QApprove(const QApprove& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QApprove::~QApprove(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprove::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprove::Init(void) {
    CTransaction::Init();

    _spender = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprove::Copy(const QApprove& ap) {
    Clear();
    CTransaction::Copy(ap);

    _spender = ap._spender;
    _value = ap._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QApprove& QApprove::operator=(const QApprove& ap) {
    Copy(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QApproveArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QApproveArray);
IMPLEMENT_ARCHIVE_LIST(QApproveList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE

