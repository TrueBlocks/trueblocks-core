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
#include <vector>
#include "abilib.h"
#include "receipt.h"
#include "trace.h"

namespace qblocks {

// EXISTING_CODE
class CBlock;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTransaction : public CBaseNode {
public:
    SFHash hash;
    SFHash blockHash;
    blknum_t blockNumber;
    uint64_t transactionIndex;
    uint64_t nonce;
    timestamp_t timestamp;
    SFAddress from;
    SFAddress to;
    SFWei value;
    SFGas gas;
    SFGas gasPrice;
    string_q input;
    uint64_t isError;
    uint64_t isInternal;
    CReceipt receipt;

public:
    CTransaction(void);
    CTransaction(const CTransaction& tr);
    virtual ~CTransaction(void);
    CTransaction& operator=(const CTransaction& tr);

    DECLARE_NODE(CTransaction);

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    const CBlock *pBlock;
    CFunction *funcPtr;
    string_q function;
    double ether;
    CTraceArray traces;

    string_q inputToFunction(void) const;
    bool operator==(const CTransaction& tr) const;
    bool operator!=(const CTransaction& tr) const { return !operator==(tr); }
    // EXISTING_CODE
    friend bool operator<(const CTransaction& v1, const CTransaction& v2);
    friend ostream& operator<<(ostream& os, const CTransaction& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransaction& tr);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    friend int sortTransactionsForWrite(const void *rr1, const void *rr2);
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransaction::CTransaction(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransaction::CTransaction(const CTransaction& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransaction::~CTransaction(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransaction::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransaction::initialize(void) {
    CBaseNode::initialize();

    hash = "";
    blockHash = "";
    blockNumber = 0;
    transactionIndex = 0;
    nonce = 0;
    timestamp = 0;
    from = "";
    to = "";
    value = 0;
    gas = 0;
    gasPrice = 0;
    input = "";
    isError = 0;
    isInternal = 0;
    receipt.initialize();

    // EXISTING_CODE
    pBlock = NULL;
    function = "";
    funcPtr = NULL;
    ether = 0.;
#if 0
    creates = "";
    confirmations = 0;
    contractAddress = "";
    r = "";
    raw = "";
    s = "";
    v = "";
    trace.Init();
#endif
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransaction::duplicate(const CTransaction& tr) {
    clear();
    CBaseNode::duplicate(tr);

    hash = tr.hash;
    blockHash = tr.blockHash;
    blockNumber = tr.blockNumber;
    transactionIndex = tr.transactionIndex;
    nonce = tr.nonce;
    timestamp = tr.timestamp;
    from = tr.from;
    to = tr.to;
    value = tr.value;
    gas = tr.gas;
    gasPrice = tr.gasPrice;
    input = tr.input;
    isError = tr.isError;
    isInternal = tr.isInternal;
    receipt = tr.receipt;

    // EXISTING_CODE
    pBlock = tr.pBlock;  // no deep copy, we don't own it
    funcPtr = tr.funcPtr;
    function = tr.function;
    ether = tr.ether;
#if 0
    creates = tr.creates;
    confirmations = tr.confirmations;
    contractAddress = tr.contractAddress;
    r = tr.r;
    raw = tr.raw;
    s = tr.s;
    v = tr.v;
    trace = tr.trace;
#endif
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CTransaction& CTransaction::operator=(const CTransaction& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransaction& v1, const CTransaction& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted
    return true;
}

//---------------------------------------------------------------------------
typedef SFArrayBase<CTransaction> CTransactionArray;
extern SFArchive& operator>>(SFArchive& archive, CTransactionArray& array);
extern SFArchive& operator<<(SFArchive& archive, const CTransactionArray& array);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CTransaction& tra);
extern SFArchive& operator>>(SFArchive& archive, CTransaction& tra);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern int sortTransactionsForWrite(const void *rr1, const void *rr2);
extern string_q parse(const string_q& params, size_t nItems, string_q *types);
extern string_q toFunction(const string_q& name, const string_q& input, size_t nItems, string_q *items);
extern string_q nextBlockChunk(const string_q& fieldIn, const void *data);
// EXISTING_CODE
}  // namespace qblocks

