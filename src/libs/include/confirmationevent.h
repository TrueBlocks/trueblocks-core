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
class QConfirmationEvent;
typedef SFArrayBase<QConfirmationEvent>         QConfirmationEventArray;
typedef SFList<QConfirmationEvent*>             QConfirmationEventList;
typedef SFUniqueList<QConfirmationEvent*>       QConfirmationEventListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QConfirmationEvent : public CLogEntry {
public:
    SFAddress owner;
    SFString operation;

public:
    QConfirmationEvent(void);
    QConfirmationEvent(const QConfirmationEvent& co);
    ~QConfirmationEvent(void);
    QConfirmationEvent& operator=(const QConfirmationEvent& co);

    DECLARE_NODE(QConfirmationEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QConfirmationEvent& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QConfirmationEvent& co);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QConfirmationEvent::QConfirmationEvent(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QConfirmationEvent::QConfirmationEvent(const QConfirmationEvent& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QConfirmationEvent::~QConfirmationEvent(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationEvent::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationEvent::Init(void) {
    CLogEntry::Init();

    owner = "";
    operation = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationEvent::Copy(const QConfirmationEvent& co) {
    Clear();
    CLogEntry::Copy(co);

    owner = co.owner;
    operation = co.operation;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QConfirmationEvent& QConfirmationEvent::operator=(const QConfirmationEvent& co) {
    Copy(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QConfirmationEventArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QConfirmationEventArray);
IMPLEMENT_ARCHIVE_LIST(QConfirmationEventList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE

