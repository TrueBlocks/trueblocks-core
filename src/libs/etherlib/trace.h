#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
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
/*
 FROM RPC DOCUMENTATION
 traceAddress field
 The traceAddress field of all returned traces, gives the exact location in the
 call trace [index in root, index in first CALL, index in second CALL, ...].
 i.e. if the trace is:
    A calls B, B calls G, A calls C, C calls G
 returns
    [ {A: []}, {B: [0]}, {G: [0, 0]}, {C: [1]}, {G: [1, 0]} ]
*/
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTrace : public CBaseNode {
public:
    SFHash blockHash;
    blknum_t blockNumber;
    uint64_t subtraces;
    SFAddressArray traceAddress;
    SFHash transactionHash;
    uint64_t transactionPosition;
    SFString type;
    SFString error;
    CTraceAction action;
    CTraceResult result;

public:
    CTrace(void);
    CTrace(const CTrace& tr);
    virtual ~CTrace(void);
    CTrace& operator=(const CTrace& tr);

    DECLARE_NODE(CTrace);

    const CBaseNode *getObjectAt(const SFString& fieldName, uint32_t index) const override;
    const SFString getStringAt(const SFString& name, uint32_t i) const override;

    // EXISTING_CODE
    bool isError(void) const;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CTrace& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CTrace& tr);
    bool readBackLevel(SFArchive& archive) override;

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
}  // namespace qblocks

