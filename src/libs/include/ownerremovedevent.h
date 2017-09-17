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
class QOwnerRemovedEvent;
typedef SFArrayBase<QOwnerRemovedEvent>         QOwnerRemovedEventArray;
typedef SFList<QOwnerRemovedEvent*>             QOwnerRemovedEventList;
typedef SFUniqueList<QOwnerRemovedEvent*>       QOwnerRemovedEventListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QOwnerRemovedEvent : public CLogEntry {
public:
    SFAddress oldOwner;

public:
    QOwnerRemovedEvent(void);
    QOwnerRemovedEvent(const QOwnerRemovedEvent& ow);
    virtual ~QOwnerRemovedEvent(void);
    QOwnerRemovedEvent& operator=(const QOwnerRemovedEvent& ow);

    DECLARE_NODE(QOwnerRemovedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QOwnerRemovedEvent& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QOwnerRemovedEvent& ow);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QOwnerRemovedEvent::QOwnerRemovedEvent(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QOwnerRemovedEvent::QOwnerRemovedEvent(const QOwnerRemovedEvent& ow) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ow);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QOwnerRemovedEvent::~QOwnerRemovedEvent(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerRemovedEvent::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerRemovedEvent::Init(void) {
    CLogEntry::Init();

    oldOwner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerRemovedEvent::Copy(const QOwnerRemovedEvent& ow) {
    Clear();
    CLogEntry::Copy(ow);

    oldOwner = ow.oldOwner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QOwnerRemovedEvent& QOwnerRemovedEvent::operator=(const QOwnerRemovedEvent& ow) {
    Copy(ow);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QOwnerRemovedEventArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QOwnerRemovedEventArray);
IMPLEMENT_ARCHIVE_LIST(QOwnerRemovedEventList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

