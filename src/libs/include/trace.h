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

namespace qblocks {

//--------------------------------------------------------------------------
class CTrace;
typedef SFArrayBase<CTrace>         CTraceArray;
typedef SFList<CTrace*>             CTraceList;
typedef SFUniqueList<CTrace*>       CTraceListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTrace : public CBaseNode {
public:
    SFHash blockHash;
    SFUint32 blockNumber;
    SFUint32 subtraces;
    SFAddressArray traceAddress;
    SFHash transactionHash;
    SFUint32 transactionPosition;
    SFString type;
    SFString error;
    CTraceAction action;
    CTraceResult result;

public:
    CTrace(void);
    CTrace(const CTrace& tr);
    ~CTrace(void);
    CTrace& operator=(const CTrace& tr);

    DECLARE_NODE(CTrace);

    // EXISTING_CODE
    bool isError(void) const;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CTrace& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CTrace& tr);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTrace::CTrace(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTrace::CTrace(const CTrace& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTrace::~CTrace(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::Init(void) {
    CBaseNode::Init();

    blockHash = "";
    blockNumber = 0;
    subtraces = 0;
    traceAddress.Clear();
    transactionHash = "";
    transactionPosition = 0;
    type = "";
    error = "";
    action.Init();
    result.Init();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::Copy(const CTrace& tr) {
    Clear();
    CBaseNode::Copy(tr);

    blockHash = tr.blockHash;
    blockNumber = tr.blockNumber;
    subtraces = tr.subtraces;
    traceAddress = tr.traceAddress;
    transactionHash = tr.transactionHash;
    transactionPosition = tr.transactionPosition;
    type = tr.type;
    error = tr.error;
    action = tr.action;
    result = tr.result;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTrace& CTrace::operator=(const CTrace& tr) {
    Copy(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CTraceArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CTraceArray);
IMPLEMENT_ARCHIVE_LIST(CTraceList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#define NEW_CODE
}  // namespace qblocks

