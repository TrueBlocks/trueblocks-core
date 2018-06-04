#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "receipt.h"
#include "trace.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CTransaction;
typedef SFArrayBase<CTransaction>         CTransactionArray;
typedef SFList<CTransaction*>             CTransactionList;
typedef SFUniqueList<CTransaction*>       CTransactionListU;

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
    SFString input;
    uint64_t isError;
    uint64_t isInternal;
    CReceipt receipt;

public:
    CTransaction(void);
    CTransaction(const CTransaction& tr);
    virtual ~CTransaction(void);
    CTransaction& operator=(const CTransaction& tr);

    DECLARE_NODE(CTransaction);

    const CBaseNode *getObjectAt(const SFString& fieldName, uint32_t index) const override;

    // EXISTING_CODE
    const CBlock *pBlock;
    CFunction *funcPtr;
    SFString function;
    double ether;
    CTraceArray traces;

    SFString inputToFunction(void) const;
    bool isFunction(const SFString& func) const;
#if 0
    SFString creates;
    uint32_t confirmations;
    SFAddress contractAddress;
    SFHash r;
    SFString raw;
    SFHash s;
    SFHash v;
    CTrace trace;
#endif
    bool operator==(const CTransaction& tr) const;
    bool operator!=(const CTransaction& tr) const { return !operator==(tr); }
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CTransaction& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CTransaction& tr);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    friend int sortTransactionsForWrite(const void *rr1, const void *rr2);
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransaction::CTransaction(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransaction::CTransaction(const CTransaction& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransaction::~CTransaction(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransaction::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransaction::Init(void) {
    CBaseNode::Init();

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
    receipt.Init();

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
inline void CTransaction::Copy(const CTransaction& tr) {
    Clear();
    CBaseNode::Copy(tr);

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
    Copy(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CTransactionArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CTransactionArray);
IMPLEMENT_ARCHIVE_LIST(CTransactionList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CTransaction& tra);
extern SFArchive& operator>>(SFArchive& archive, CTransaction& tra);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern int sortTransactionsForWrite(const void *rr1, const void *rr2);
extern SFString parse(const SFString& params, uint32_t nItems, SFString *types);
extern SFString toFunction(const SFString& name, const SFString& input, uint32_t nItems, SFString *items);
extern SFString nextBlockChunk(const SFString& fieldIn, const void *data);
// EXISTING_CODE
}  // namespace qblocks

