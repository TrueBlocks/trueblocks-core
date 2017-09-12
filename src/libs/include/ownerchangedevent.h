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
class QOwnerChangedEvent;
typedef SFArrayBase<QOwnerChangedEvent>         QOwnerChangedEventArray;
typedef SFList<QOwnerChangedEvent*>             QOwnerChangedEventList;
typedef SFUniqueList<QOwnerChangedEvent*>       QOwnerChangedEventListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QOwnerChangedEvent : public CLogEntry {
public:
    SFAddress oldOwner;
    SFAddress newOwner;

public:
    QOwnerChangedEvent(void);
    QOwnerChangedEvent(const QOwnerChangedEvent& ow);
    ~QOwnerChangedEvent(void);
    QOwnerChangedEvent& operator=(const QOwnerChangedEvent& ow);

    DECLARE_NODE(QOwnerChangedEvent);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QOwnerChangedEvent& ow);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QOwnerChangedEvent::QOwnerChangedEvent(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QOwnerChangedEvent::QOwnerChangedEvent(const QOwnerChangedEvent& ow) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ow);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QOwnerChangedEvent::~QOwnerChangedEvent(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerChangedEvent::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerChangedEvent::Init(void) {
    CLogEntry::Init();

    oldOwner = "";
    newOwner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerChangedEvent::Copy(const QOwnerChangedEvent& ow) {
    Clear();
    CLogEntry::Copy(ow);

    oldOwner = ow.oldOwner;
    newOwner = ow.newOwner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QOwnerChangedEvent& QOwnerChangedEvent::operator=(const QOwnerChangedEvent& ow) {
    Copy(ow);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QOwnerChangedEventArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QOwnerChangedEventArray);
IMPLEMENT_ARCHIVE_LIST(QOwnerChangedEventList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE

