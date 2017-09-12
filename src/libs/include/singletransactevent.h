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
class QSingleTransactEvent;
typedef SFArrayBase<QSingleTransactEvent>         QSingleTransactEventArray;
typedef SFList<QSingleTransactEvent*>             QSingleTransactEventList;
typedef SFUniqueList<QSingleTransactEvent*>       QSingleTransactEventListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QSingleTransactEvent : public CLogEntry {
public:
    SFAddress owner;
    SFUintBN value;
    SFAddress to;
    SFString data;

public:
    QSingleTransactEvent(void);
    QSingleTransactEvent(const QSingleTransactEvent& si);
    ~QSingleTransactEvent(void);
    QSingleTransactEvent& operator=(const QSingleTransactEvent& si);

    DECLARE_NODE(QSingleTransactEvent);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QSingleTransactEvent& si);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QSingleTransactEvent::QSingleTransactEvent(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QSingleTransactEvent::QSingleTransactEvent(const QSingleTransactEvent& si) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(si);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QSingleTransactEvent::~QSingleTransactEvent(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSingleTransactEvent::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSingleTransactEvent::Init(void) {
    CLogEntry::Init();

    owner = "";
    value = 0;
    to = "";
    data = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSingleTransactEvent::Copy(const QSingleTransactEvent& si) {
    Clear();
    CLogEntry::Copy(si);

    owner = si.owner;
    value = si.value;
    to = si.to;
    data = si.data;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QSingleTransactEvent& QSingleTransactEvent::operator=(const QSingleTransactEvent& si) {
    Copy(si);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QSingleTransactEventArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QSingleTransactEventArray);
IMPLEMENT_ARCHIVE_LIST(QSingleTransactEventList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE

