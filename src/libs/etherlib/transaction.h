#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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

// EXISTING_CODE
class CBlock;
class CAppearance;
typedef bool (*ADDRESSFUNC)(const CAppearance& item, void *data);
typedef bool (*TRANSFUNC)(const CTransaction *trans, void *data);
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTransaction : public CBaseNode {
public:
    hash_t hash;
    hash_t blockHash;
    blknum_t blockNumber;
    uint64_t transactionIndex;
    uint64_t nonce;
    timestamp_t timestamp;
    address_t from;
    address_t to;
    wei_t value;
    gas_t gas;
    gas_t gasPrice;
    string_q input;
    uint64_t isError;
    uint64_t isInternal;
    CReceipt receipt;
    CFunction articulatedTx;
    string_q compressedTx;
    bool finalized;
    CTraceArray traces;

public:
    CTransaction(void);
    CTransaction(const CTransaction& tr);
    virtual ~CTransaction(void);
    CTransaction& operator=(const CTransaction& tr);

    DECLARE_NODE(CTransaction);

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    const CBlock *pBlock;
    bool forEveryAddress(ADDRESSFUNC func, TRANSFUNC filt = NULL, void *data = NULL);
    bool forEveryUniqueAddress(ADDRESSFUNC func, TRANSFUNC filt = NULL, void *data = NULL);
    bool forEveryUniqueAddressPerTx(ADDRESSFUNC func, TRANSFUNC filt = NULL, void *data = NULL);
    bool loadAsPrefund(const CStringArray& prefunds, const address_t& addr);
    bool loadAsBlockReward(blknum_t bn, blknum_t txid, const address_t& addr);
    // EXISTING_CODE
    bool operator==(const CTransaction& item) const;
    bool operator!=(const CTransaction& item) const { return !operator==(item); }
    friend bool operator<(const CTransaction& v1, const CTransaction& v2);
    friend ostream& operator<<(ostream& os, const CTransaction& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransaction& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    friend class CAccount;
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
    receipt = CReceipt();
    articulatedTx = CFunction();
    compressedTx = "";
    finalized = 0;
    traces.clear();

    // EXISTING_CODE
    pBlock = NULL;
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
    articulatedTx = tr.articulatedTx;
    compressedTx = tr.compressedTx;
    finalized = tr.finalized;
    traces = tr.traces;

    // EXISTING_CODE
    pBlock = tr.pBlock;  // no deep copy, we don't own it
    finishParse();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransaction& CTransaction::operator=(const CTransaction& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTransaction::operator==(const CTransaction& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTransaction& v1, const CTransaction& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTransaction> CTransactionArray;
extern CArchive& operator>>(CArchive& archive, CTransactionArray& array);
extern CArchive& operator<<(CArchive& archive, const CTransactionArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CTransaction& tra);
extern CArchive& operator>>(CArchive& archive, CTransaction& tra);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern bool sortTransactionsForWrite(const CTransaction& t1, const CTransaction& t2);
extern string_q nextBlockChunk(const string_q& fieldIn, const void *data);
// EXISTING_CODE
}  // namespace qblocks

