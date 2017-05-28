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
class QRequirementChangedEvent;
typedef SFArrayBase<QRequirementChangedEvent>         QRequirementChangedEventArray;
typedef SFList<QRequirementChangedEvent*>             QRequirementChangedEventList;
typedef SFUniqueList<QRequirementChangedEvent*>       QRequirementChangedEventListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QRequirementChangedEvent : public CLogEntry {
public:
    SFUintBN newRequirement;

public:
    QRequirementChangedEvent(void);
    QRequirementChangedEvent(const QRequirementChangedEvent& re);
    ~QRequirementChangedEvent(void);
    QRequirementChangedEvent& operator=(const QRequirementChangedEvent& re);

    DECLARE_NODE(QRequirementChangedEvent);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QRequirementChangedEvent& re);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QRequirementChangedEvent::QRequirementChangedEvent(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QRequirementChangedEvent::QRequirementChangedEvent(const QRequirementChangedEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QRequirementChangedEvent::~QRequirementChangedEvent(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRequirementChangedEvent::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRequirementChangedEvent::Init(void) {
    CLogEntry::Init();

    newRequirement = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRequirementChangedEvent::Copy(const QRequirementChangedEvent& re) {
    Clear();
    CLogEntry::Copy(re);

    newRequirement = re.newRequirement;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QRequirementChangedEvent& QRequirementChangedEvent::operator=(const QRequirementChangedEvent& re) {
    Copy(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString QRequirementChangedEvent::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QRequirementChangedEventArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QRequirementChangedEventArray);
IMPLEMENT_ARCHIVE_LIST(QRequirementChangedEventList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

