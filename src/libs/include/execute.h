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
class QExecute;
typedef SFArrayBase<QExecute>         QExecuteArray;
typedef SFList<QExecute*>             QExecuteList;
typedef SFUniqueList<QExecute*>       QExecuteListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QExecute : public CTransaction {
public:
    SFAddress _to;
    SFUintBN _value;
    SFString _data;

public:
    QExecute(void);
    QExecute(const QExecute& ex);
    ~QExecute(void);
    QExecute& operator=(const QExecute& ex);

    DECLARE_NODE(QExecute);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QExecute& ex);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QExecute::QExecute(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QExecute::QExecute(const QExecute& ex) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ex);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QExecute::~QExecute(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QExecute::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QExecute::Init(void) {
    CTransaction::Init();

//    _to = EMPTY;
    _value = 0;
//    _data = EMPTY;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QExecute::Copy(const QExecute& ex) {
    Clear();
    CTransaction::Copy(ex);

    _to = ex._to;
    _value = ex._value;
    _data = ex._data;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QExecute& QExecute::operator=(const QExecute& ex) {
    Copy(ex);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString QExecute::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QExecuteArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QExecuteArray);
IMPLEMENT_ARCHIVE_LIST(QExecuteList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

