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
class QRevoke;
typedef SFArrayBase<QRevoke>         QRevokeArray;
typedef SFList<QRevoke*>             QRevokeList;
typedef SFUniqueList<QRevoke*>       QRevokeListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QRevoke : public CTransaction
{
public:
    SFString _operation;

public:
    QRevoke(void);
    QRevoke(const QRevoke& re);
    ~QRevoke(void);
    QRevoke& operator=(const QRevoke& re);

    DECLARE_NODE (QRevoke);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QRevoke& re);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QRevoke::QRevoke(void)
{
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QRevoke::QRevoke(const QRevoke& re)
{
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QRevoke::~QRevoke(void)
{
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevoke::Clear(void)
{
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevoke::Init(void)
{
    CTransaction::Init();

//    _operation = EMPTY;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QRevoke::Copy(const QRevoke& re)
{
    Clear();
    CTransaction::Copy(re);

    _operation = re._operation;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QRevoke& QRevoke::operator=(const QRevoke& re)
{
    Copy(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString QRevoke::getValueByName(const SFString& fieldName) const
{
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QRevokeArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QRevokeArray);
IMPLEMENT_ARCHIVE_LIST(QRevokeList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

