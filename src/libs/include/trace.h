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
#include "ethtypes.h"
#include "abilib.h"
#include "traceaction.h"
#include "traceresult.h"

//--------------------------------------------------------------------------
class CTrace;
typedef SFArrayBase<CTrace>         CTraceArray;
typedef SFList<CTrace*>             CTraceList;
typedef SFUniqueList<CTrace*>       CTraceListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTrace : public CBaseNode
{
public:
    CTraceAction action;
    SFHash blockHash;
    SFUint32 blockNumber;
    CTraceResult result;
    SFUint32 subtraces;
    SFStringArray traceAddress;
    SFHash transactionHash;
    SFUint32 transactionPosition;
    SFString type;

public:
    CTrace(void);
    CTrace(const CTrace& tr);
   ~CTrace(void);
    CTrace& operator=(const CTrace& tr);

    DECLARE_NODE (CTrace);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CTrace& tr);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTrace::CTrace(void)
{
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTrace::CTrace(const CTrace& tr)
{
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTrace::~CTrace(void)
{
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::Clear(void)
{
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::Init(void)
{
    CBaseNode::Init();

//    action = ??; /* unknown type: CTraceAction */
//    blockHash = EMPTY;
    blockNumber = 0;
//    result = ??; /* unknown type: CTraceResult */
    subtraces = 0;
//    traceAddress = ??; /* unknown type: SFAddressArray */
//    transactionHash = EMPTY;
    transactionPosition = 0;
//    type = EMPTY;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::Copy(const CTrace& tr)
{
    Clear();
    CBaseNode::Copy(tr);

    action = tr.action;
    blockHash = tr.blockHash;
    blockNumber = tr.blockNumber;
    result = tr.result;
    subtraces = tr.subtraces;
    traceAddress = tr.traceAddress;
    transactionHash = tr.transactionHash;
    transactionPosition = tr.transactionPosition;
    type = tr.type;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTrace& CTrace::operator=(const CTrace& tr)
{
    Copy(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString CTrace::getValueByName(const SFString& fieldName) const
{
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CTraceArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CTraceArray);
IMPLEMENT_ARCHIVE_LIST(CTraceList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
