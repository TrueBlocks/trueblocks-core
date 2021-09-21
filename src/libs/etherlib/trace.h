#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "utillib.h"
#include "abi.h"
#include "traceaction.h"
#include "traceresult.h"

namespace qblocks {

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
    hash_t blockHash;
    blknum_t blockNumber;
    uint64_t subtraces;
    CStringArray traceAddress;
    hash_t transactionHash;
    blknum_t transactionIndex;
    string_q type;
    string_q error;
    CTraceAction action;
    CTraceResult result;
    CFunction articulatedTrace;
    string_q compressedTrace;

  public:
    CTrace(void);
    CTrace(const CTrace& tr);
    virtual ~CTrace(void);
    CTrace& operator=(const CTrace& tr);

    DECLARE_NODE(CTrace);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;
    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    bool isErr(void) const;
    const CTransaction* pTransaction;
    void loadTraceAsBlockReward(const CTransaction& trans, blknum_t bn, blknum_t txid);
    void loadTraceAsUncleReward(const CTransaction& trans, blknum_t bn, blknum_t uncleBn);
    void loadTraceAsTransFee(const CTransaction& trans, blknum_t bn, blknum_t txid);
    void loadTraceAsDdos(const CTransaction& trans, blknum_t bn, blknum_t txid);
    // EXISTING_CODE
    bool operator==(const CTrace& it) const;
    bool operator!=(const CTrace& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTrace& v1, const CTrace& v2);
    friend ostream& operator<<(ostream& os, const CTrace& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTrace& tr);
    bool readBackLevel(CArchive& archive) override;

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
    transactionIndex = 0;
    type = "";
    error = "";
    action = CTraceAction();
    result = CTraceResult();
    articulatedTrace = CFunction();
    compressedTrace = "";

    // EXISTING_CODE
    pTransaction = NULL;
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
    transactionIndex = tr.transactionIndex;
    type = tr.type;
    error = tr.error;
    action = tr.action;
    result = tr.result;
    articulatedTrace = tr.articulatedTrace;
    compressedTrace = tr.compressedTrace;

    // EXISTING_CODE
    pTransaction = tr.pTransaction;  // no deep copy, we don't own it
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTrace& CTrace::operator=(const CTrace& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTrace::operator==(const CTrace& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTrace& v1, const CTrace& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTrace> CTraceArray;
extern CArchive& operator>>(CArchive& archive, CTraceArray& array);
extern CArchive& operator<<(CArchive& archive, const CTraceArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CTrace& tra);
extern CArchive& operator>>(CArchive& archive, CTrace& tra);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TRACE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
