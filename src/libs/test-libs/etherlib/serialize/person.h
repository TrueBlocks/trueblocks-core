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
#include "etherlib.h"

//--------------------------------------------------------------------------
class CPerson;
typedef SFArrayBase<CPerson>         CPersonArray;
typedef SFList<CPerson*>             CPersonList;
typedef SFUniqueList<CPerson*>       CPersonListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPerson : public CBaseNode {
public:
    SFString name;
    SFUint32 age;
    CPerson *next;

public:
    CPerson(void);
    CPerson(const CPerson& pe);
    virtual ~CPerson(void);
    CPerson& operator=(const CPerson& pe);

    DECLARE_NODE(CPerson);

    // EXISTING_CODE
    CPerson(const SFString& n, SFUint32 a) : name(n), age(a), next(NULL) { }
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CPerson& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CPerson& pe);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPerson::CPerson(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPerson::CPerson(const CPerson& pe) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(pe);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPerson::~CPerson(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPerson::Clear(void) {
    if (next)
        delete next;
    next = NULL;
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPerson::Init(void) {
    CBaseNode::Init();

    name = "";
    age = 0;
    next = NULL;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPerson::Copy(const CPerson& pe) {
    Clear();
    CBaseNode::Copy(pe);

    name = pe.name;
    age = pe.age;
    if (pe.next) {
        next = new CPerson;
        *next = *pe.next;
    }

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CPerson& CPerson::operator=(const CPerson& pe) {
    Copy(pe);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CPersonArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CPersonArray);
IMPLEMENT_ARCHIVE_LIST(CPersonList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

