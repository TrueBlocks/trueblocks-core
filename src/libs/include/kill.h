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
class QKill;
typedef SFArrayBase<QKill>         QKillArray;
typedef SFList<QKill*>             QKillList;
typedef SFUniqueList<QKill*>       QKillListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QKill : public CTransaction
{
public:
    SFAddress _to;

public:
    QKill(void);
    QKill(const QKill& ki);
    ~QKill(void);
    QKill& operator=(const QKill& ki);

    DECLARE_NODE (QKill);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QKill& ki);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QKill::QKill(void)
{
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QKill::QKill(const QKill& ki)
{
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ki);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QKill::~QKill(void)
{
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QKill::Clear(void)
{
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QKill::Init(void)
{
    CTransaction::Init();

//    _to = EMPTY;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QKill::Copy(const QKill& ki)
{
    Clear();
    CTransaction::Copy(ki);

    _to = ki._to;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QKill& QKill::operator=(const QKill& ki)
{
    Copy(ki);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString QKill::getValueByName(const SFString& fieldName) const
{
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QKillArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QKillArray);
IMPLEMENT_ARCHIVE_LIST(QKillList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

