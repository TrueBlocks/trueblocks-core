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
class QSetDailyLimit;
typedef SFArrayBase<QSetDailyLimit>         QSetDailyLimitArray;
typedef SFList<QSetDailyLimit*>             QSetDailyLimitList;
typedef SFUniqueList<QSetDailyLimit*>       QSetDailyLimitListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QSetDailyLimit : public CTransaction {
public:
    SFUintBN _newLimit;

public:
    QSetDailyLimit(void);
    QSetDailyLimit(const QSetDailyLimit& se);
    ~QSetDailyLimit(void);
    QSetDailyLimit& operator=(const QSetDailyLimit& se);

    DECLARE_NODE(QSetDailyLimit);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QSetDailyLimit& se);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QSetDailyLimit::QSetDailyLimit(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QSetDailyLimit::QSetDailyLimit(const QSetDailyLimit& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QSetDailyLimit::~QSetDailyLimit(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSetDailyLimit::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSetDailyLimit::Init(void) {
    CTransaction::Init();

    _newLimit = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSetDailyLimit::Copy(const QSetDailyLimit& se) {
    Clear();
    CTransaction::Copy(se);

    _newLimit = se._newLimit;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QSetDailyLimit& QSetDailyLimit::operator=(const QSetDailyLimit& se) {
    Copy(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QSetDailyLimitArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QSetDailyLimitArray);
IMPLEMENT_ARCHIVE_LIST(QSetDailyLimitList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE

