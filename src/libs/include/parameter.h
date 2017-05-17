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
#include "utillib.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CParameter;
typedef SFArrayBase<CParameter>         CParameterArray;
typedef SFList<CParameter*>             CParameterList;
typedef SFUniqueList<CParameter*>       CParameterListU;

// EXISTING_CODE
class CFunction;
//--------------------------------------------------------------------------
#define SIG_FTYPE     (1<<1)
#define SIG_FNAME     (1<<2)
#define SIG_FSPACE    (1<<3)
#define SIG_ITYPE     (1<<4)
#define SIG_INAME     (1<<5)
#define SIG_IINDEXED  (1<<6)
#define SIG_ENCODE    (1<<7)
#define SIG_CONST     (1<<8)
#define SIG_ANONYMOUS (1<<9)
#define SIG_PAYABLE   (1<<10)
#define SIG_CANONICAL (SIG_FNAME|SIG_ITYPE)
#define SIG_DEFAULT   (SIG_FTYPE|SIG_FNAME|SIG_FSPACE|SIG_ITYPE|SIG_INAME)
#define SIG_DETAILS   (SIG_DEFAULT|SIG_IINDEXED|SIG_CONST|SIG_ANONYMOUS|SIG_PAYABLE|SIG_ENCODE)
// EXISTING_CODE

//--------------------------------------------------------------------------
class CParameter : public CBaseNode
{
public:
    bool indexed;
    SFString name;
    SFString type;
    bool isPointer;
    bool isArray;
    SFString strDefault;

public:
    CParameter(void);
    CParameter(const CParameter& pa);
    ~CParameter(void);
    CParameter& operator=(const CParameter& pa);

    DECLARE_NODE (CParameter);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CParameter& pa);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CParameter::CParameter(void)
{
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CParameter::CParameter(const CParameter& pa)
{
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(pa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CParameter::~CParameter(void)
{
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CParameter::Clear(void)
{
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CParameter::Init(void)
{
    CBaseNode::Init();

    indexed = 0;
//    name = EMPTY;
//    type = EMPTY;
    isPointer = 0;
    isArray = 0;
//    strDefault = EMPTY;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CParameter::Copy(const CParameter& pa)
{
    Clear();
    CBaseNode::Copy(pa);

    indexed = pa.indexed;
    name = pa.name;
    type = pa.type;
    isPointer = pa.isPointer;
    isArray = pa.isArray;
    strDefault = pa.strDefault;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CParameter& CParameter::operator=(const CParameter& pa)
{
    Copy(pa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString CParameter::getValueByName(const SFString& fieldName) const
{
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CParameterArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CParameterArray);
IMPLEMENT_ARCHIVE_LIST(CParameterList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

