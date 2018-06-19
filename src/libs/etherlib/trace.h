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
    string_q type;
    string_q error;
    CTraceAction action;
    CTraceResult result;

public:
    CTrace(void);
    CTrace(const CTrace& tr);
    virtual ~CTrace(void);
    CTrace& operator=(const CTrace& tr);

    DECLARE_NODE(CTrace);

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;
    const string_q getStringAt(const string_q& name, size_t i) const override;

    // EXISTING_CODE
    bool isError(void) const;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CTrace& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTrace& tr);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTrace::CTrace(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTrace::CTrace(const CTrace& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTrace::~CTrace(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::initialize(void) {
    CBaseNode::initialize();

    blockHash = "";
    blockNumber = 0;
    subtraces = 0;
    traceAddress.clear();
    transactionHash = "";
    transactionPosition = 0;
    type = "";
    error = "";
    action.initialize();
    result.initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::duplicate(const CTrace& tr) {
    clear();
    CBaseNode::duplicate(tr);

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
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
extern SFArchive& operator>>(SFArchive& archive, CTraceArray& array);
extern SFArchive& operator<<(SFArchive& archive, const CTraceArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

