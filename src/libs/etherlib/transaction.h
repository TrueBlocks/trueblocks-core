#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include "reconciliation.h"
#include "reconciliationoutput.h"
#include "ethstate.h"

namespace qblocks {

// EXISTING_CODE
class CBlock;
class CAppearance;
typedef bool (*ADDRESSFUNC)(const CAppearance& item, void* data);
typedef bool (*TRANSFUNC)(const CTransaction* trans, void* data);
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTransaction : public CBaseNode {
  public:
    hash_t hash;
    hash_t blockHash;
    blknum_t blockNumber;
    blknum_t transactionIndex;
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
    CTraceArray traces;
    CFunction articulatedTx;
    CReconciliationArray reconciliations;
    string_q compressedTx;
    CReconciliationOutputArray statements;
    bool finalized;

  public:
    CTransaction(void);
    CTransaction(const CTransaction& tr);
    virtual ~CTransaction(void);
    CTransaction& operator=(const CTransaction& tr);

    DECLARE_NODE(CTransaction);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    wei_t extraValue1;
    wei_t extraValue2;
    const CBlock* pBlock;
    bool forEveryAddressTx(ADDRESSFUNC func, TRANSFUNC filt = NULL, void* data = NULL);
    bool forEveryUniqueAddress(ADDRESSFUNC func, TRANSFUNC filt = NULL, void* data = NULL);
    bool forEveryUniqueAddressPerTx(ADDRESSFUNC func, TRANSFUNC filt = NULL, void* data = NULL);
    bool loadTransAsPrefund(const address_t& addr, const wei_t& amount);
    bool loadTransAsBlockReward(blknum_t bn, blknum_t txid, const address_t& addr);
    bool loadTransAsUncleReward(blknum_t bn, blknum_t uncleBn);
    // EXISTING_CODE
    bool operator==(const CTransaction& item) const;
    bool operator!=(const CTransaction& item) const {
        return !operator==(item);
    }
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
    traces.clear();
    articulatedTx = CFunction();
    reconciliations.clear();
    compressedTx = "";
    statements.clear();
    finalized = false;

    // EXISTING_CODE
    pBlock = NULL;
    extraValue1 = 0;
    extraValue2 = 0;
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
    traces = tr.traces;
    articulatedTx = tr.articulatedTx;
    reconciliations = tr.reconciliations;
    compressedTx = tr.compressedTx;
    statements = tr.statements;
    finalized = tr.finalized;

    // EXISTING_CODE
    pBlock = tr.pBlock;  // no deep copy, we don't own it
    extraValue1 = tr.extraValue1;
    extraValue2 = tr.extraValue2;
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
extern const char* STR_DISPLAY_TRANSACTION;

//---------------------------------------------------------------------------
// EXISTING_CODE
extern bool sortTransactionsForWrite(const CTransaction& t1, const CTransaction& t2);
extern string_q nextBlockChunk(const string_q& fieldIn, const void* data);
// EXISTING_CODE
}  // namespace qblocks
