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
#include "transaction.h"

//--------------------------------------------------------------------------
class QVersion;
typedef SFArrayBase<QVersion>         QVersionArray;
typedef SFList<QVersion*>             QVersionList;
typedef SFUniqueList<QVersion*>       QVersionListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QVersion : public CTransaction {
public:

public:
    QVersion(void);
    QVersion(const QVersion& ve);
    virtual ~QVersion(void);
    QVersion& operator=(const QVersion& ve);

    DECLARE_NODE(QVersion);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QVersion& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QVersion& ve);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QVersion::QVersion(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QVersion::QVersion(const QVersion& ve) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ve);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QVersion::~QVersion(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QVersion::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QVersion::Init(void) {
    CTransaction::Init();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QVersion::Copy(const QVersion& ve) {
    Clear();
    CTransaction::Copy(ve);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QVersion& QVersion::operator=(const QVersion& ve) {
    Copy(ve);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QVersionArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QVersionArray);
IMPLEMENT_ARCHIVE_LIST(QVersionList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

