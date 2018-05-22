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
class QApproveAndCall;
typedef SFArrayBase<QApproveAndCall>         QApproveAndCallArray;
typedef SFList<QApproveAndCall*>             QApproveAndCallList;
typedef SFUniqueList<QApproveAndCall*>       QApproveAndCallListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QApproveAndCall : public CTransaction {
public:
    SFAddress _spender;
    SFUintBN _value;
    SFString _extraData;

public:
    QApproveAndCall(void);
    QApproveAndCall(const QApproveAndCall& ap);
    virtual ~QApproveAndCall(void);
    QApproveAndCall& operator=(const QApproveAndCall& ap);

    DECLARE_NODE(QApproveAndCall);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QApproveAndCall& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QApproveAndCall& ap);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QApproveAndCall::QApproveAndCall(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QApproveAndCall::QApproveAndCall(const QApproveAndCall& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QApproveAndCall::~QApproveAndCall(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApproveAndCall::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApproveAndCall::Init(void) {
    CTransaction::Init();

    _spender = "";
    _value = 0;
    _extraData = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApproveAndCall::Copy(const QApproveAndCall& ap) {
    Clear();
    CTransaction::Copy(ap);

    _spender = ap._spender;
    _value = ap._value;
    _extraData = ap._extraData;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QApproveAndCall& QApproveAndCall::operator=(const QApproveAndCall& ap) {
    Copy(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QApproveAndCallArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QApproveAndCallArray);
IMPLEMENT_ARCHIVE_LIST(QApproveAndCallList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

