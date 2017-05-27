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
class QMultiTransactEvent;
typedef SFArrayBase<QMultiTransactEvent>         QMultiTransactEventArray;
typedef SFList<QMultiTransactEvent*>             QMultiTransactEventList;
typedef SFUniqueList<QMultiTransactEvent*>       QMultiTransactEventListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QMultiTransactEvent : public CLogEntry
{
public:
    SFAddress owner;
    SFString operation;
    SFUintBN value;
    SFAddress to;
    SFString data;

public:
    QMultiTransactEvent(void);
    QMultiTransactEvent(const QMultiTransactEvent& mu);
    ~QMultiTransactEvent(void);
    QMultiTransactEvent& operator=(const QMultiTransactEvent& mu);

    DECLARE_NODE (QMultiTransactEvent);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QMultiTransactEvent& mu);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QMultiTransactEvent::QMultiTransactEvent(void)
{
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QMultiTransactEvent::QMultiTransactEvent(const QMultiTransactEvent& mu)
{
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(mu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QMultiTransactEvent::~QMultiTransactEvent(void)
{
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QMultiTransactEvent::Clear(void)
{
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QMultiTransactEvent::Init(void)
{
    CLogEntry::Init();

//    owner = EMPTY;
//    operation = EMPTY;
    value = 0;
//    to = EMPTY;
//    data = EMPTY;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QMultiTransactEvent::Copy(const QMultiTransactEvent& mu)
{
    Clear();
    CLogEntry::Copy(mu);

    owner = mu.owner;
    operation = mu.operation;
    value = mu.value;
    to = mu.to;
    data = mu.data;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QMultiTransactEvent& QMultiTransactEvent::operator=(const QMultiTransactEvent& mu)
{
    Copy(mu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString QMultiTransactEvent::getValueByName(const SFString& fieldName) const
{
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QMultiTransactEventArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QMultiTransactEventArray);
IMPLEMENT_ARCHIVE_LIST(QMultiTransactEventList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

