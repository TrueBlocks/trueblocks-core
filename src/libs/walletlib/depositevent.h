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
class QDepositEvent;
typedef SFArrayBase<QDepositEvent>         QDepositEventArray;
typedef SFList<QDepositEvent*>             QDepositEventList;
typedef SFUniqueList<QDepositEvent*>       QDepositEventListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QDepositEvent : public CLogEntry {
public:
    SFAddress from;
    SFUintBN value;

public:
    QDepositEvent(void);
    QDepositEvent(const QDepositEvent& de);
    ~QDepositEvent(void);
    QDepositEvent& operator=(const QDepositEvent& de);

    DECLARE_NODE(QDepositEvent);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QDepositEvent& de);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QDepositEvent::QDepositEvent(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QDepositEvent::QDepositEvent(const QDepositEvent& de) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(de);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QDepositEvent::~QDepositEvent(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QDepositEvent::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QDepositEvent::Init(void) {
    CLogEntry::Init();

    from = "";
    value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QDepositEvent::Copy(const QDepositEvent& de) {
    Clear();
    CLogEntry::Copy(de);

    from = de.from;
    value = de.value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QDepositEvent& QDepositEvent::operator=(const QDepositEvent& de) {
    Copy(de);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString QDepositEvent::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QDepositEventArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QDepositEventArray);
IMPLEMENT_ARCHIVE_LIST(QDepositEventList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

