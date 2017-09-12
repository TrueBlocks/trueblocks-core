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
class QConfirmationNeededEvent;
typedef SFArrayBase<QConfirmationNeededEvent>         QConfirmationNeededEventArray;
typedef SFList<QConfirmationNeededEvent*>             QConfirmationNeededEventList;
typedef SFUniqueList<QConfirmationNeededEvent*>       QConfirmationNeededEventListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QConfirmationNeededEvent : public CLogEntry {
public:
    SFString operation;
    SFAddress initiator;
    SFUintBN value;
    SFAddress to;
    SFString data;

public:
    QConfirmationNeededEvent(void);
    QConfirmationNeededEvent(const QConfirmationNeededEvent& co);
    ~QConfirmationNeededEvent(void);
    QConfirmationNeededEvent& operator=(const QConfirmationNeededEvent& co);

    DECLARE_NODE(QConfirmationNeededEvent);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QConfirmationNeededEvent& co);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QConfirmationNeededEvent::QConfirmationNeededEvent(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QConfirmationNeededEvent::QConfirmationNeededEvent(const QConfirmationNeededEvent& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QConfirmationNeededEvent::~QConfirmationNeededEvent(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationNeededEvent::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationNeededEvent::Init(void) {
    CLogEntry::Init();

    operation = "";
    initiator = "";
    value = 0;
    to = "";
    data = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QConfirmationNeededEvent::Copy(const QConfirmationNeededEvent& co) {
    Clear();
    CLogEntry::Copy(co);

    operation = co.operation;
    initiator = co.initiator;
    value = co.value;
    to = co.to;
    data = co.data;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QConfirmationNeededEvent& QConfirmationNeededEvent::operator=(const QConfirmationNeededEvent& co) {
    Copy(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QConfirmationNeededEventArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QConfirmationNeededEventArray);
IMPLEMENT_ARCHIVE_LIST(QConfirmationNeededEventList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE

