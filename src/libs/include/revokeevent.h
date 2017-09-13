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
class QRevokeEvent;
typedef SFArrayBase<QRevokeEvent>         QRevokeEventArray;
typedef SFList<QRevokeEvent*>             QRevokeEventList;
typedef SFUniqueList<QRevokeEvent*>       QRevokeEventListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QRevokeEvent : public CLogEntry {
public:
    SFAddress owner;
    SFString operation;

public:
    QRevokeEvent(void);
    QRevokeEvent(const QRevokeEvent& re);
    ~QRevokeEvent(void);
    QRevokeEvent& operator=(const QRevokeEvent& re);

    DECLARE_NODE(QRevokeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QRevokeEvent& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QRevokeEvent& re);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QRevokeEvent::QRevokeEvent(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QRevokeEvent::QRevokeEvent(const QRevokeEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QRevokeEvent::~QRevokeEvent(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevokeEvent::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevokeEvent::Init(void) {
    CLogEntry::Init();

    owner = "";
    operation = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevokeEvent::Copy(const QRevokeEvent& re) {
    Clear();
    CLogEntry::Copy(re);

    owner = re.owner;
    operation = re.operation;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QRevokeEvent& QRevokeEvent::operator=(const QRevokeEvent& re) {
    Copy(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QRevokeEventArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QRevokeEventArray);
IMPLEMENT_ARCHIVE_LIST(QRevokeEventList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE

