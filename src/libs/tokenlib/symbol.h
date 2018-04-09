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
class QSymbol;
typedef SFArrayBase<QSymbol>         QSymbolArray;
typedef SFList<QSymbol*>             QSymbolList;
typedef SFUniqueList<QSymbol*>       QSymbolListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QSymbol : public CTransaction {
public:

public:
    QSymbol(void);
    QSymbol(const QSymbol& sy);
    virtual ~QSymbol(void);
    QSymbol& operator=(const QSymbol& sy);

    DECLARE_NODE(QSymbol);

    // EXISTING_CODE
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const QSymbol& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const QSymbol& sy);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QSymbol::QSymbol(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QSymbol::QSymbol(const QSymbol& sy) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(sy);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QSymbol::~QSymbol(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSymbol::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSymbol::Init(void) {
    CTransaction::Init();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QSymbol::Copy(const QSymbol& sy) {
    Clear();
    CTransaction::Copy(sy);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QSymbol& QSymbol::operator=(const QSymbol& sy) {
    Copy(sy);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(QSymbolArray);
IMPLEMENT_ARCHIVE_ARRAY_C(QSymbolArray);
IMPLEMENT_ARCHIVE_LIST(QSymbolList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

