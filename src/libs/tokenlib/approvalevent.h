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
#include "logentry.h"

//--------------------------------------------------------------------------
class QApprovalEvent;
typedef SFArrayBase<QApprovalEvent>         QApprovalEventArray;
typedef SFList<QApprovalEvent*>             QApprovalEventList;
typedef SFUniqueList<QApprovalEvent*>       QApprovalEventListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QApprovalEvent : public CLogEntry {
public:
    SFAddress _owner;
    SFAddress _spender;
    SFUintBN _value;

public:
    QApprovalEvent(void);
    QApprovalEvent(const QApprovalEvent& ap);
    virtual ~QApprovalEvent(void);
    QApprovalEvent& operator=(const QApprovalEvent& ap);

    DECLARE_NODE(QApprovalEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QApprovalEvent& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QApprovalEvent& ap);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QApprovalEvent::QApprovalEvent(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QApprovalEvent::QApprovalEvent(const QApprovalEvent& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QApprovalEvent::~QApprovalEvent(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprovalEvent::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprovalEvent::Init(void) {
    CLogEntry::Init();

    _owner = "";
    _spender = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprovalEvent::Copy(const QApprovalEvent& ap) {
    Clear();
    CLogEntry::Copy(ap);

    _owner = ap._owner;
    _spender = ap._spender;
    _value = ap._value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QApprovalEvent& QApprovalEvent::operator=(const QApprovalEvent& ap) {
    Copy(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QApprovalEventArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QApprovalEventArray);
IMPLEMENT_ARCHIVE_LIST(QApprovalEventList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

