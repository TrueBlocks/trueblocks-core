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
#include "receipt.h"
#include "trace.h"
#include "ethstate.h"
#include "ledgermanager.h"

namespace qblocks {

// EXISTING_CODE
class CBlock;
class CAppearance;
typedef bool (*APPEARANCEFUNC)(const CAppearance& item, void* data);
typedef bool (*TRANSFUNC)(const CTransaction* trans, void* data);
typedef bool (*LOGVISITFUNC)(CLogEntry& log, void* data);
typedef bool (*TRACEVISITFUNC)(CTrace& trace, void* data);
typedef enum {
    CB_NONE = 0,
    CB_ARTICULATION = (1 << 0),
    CB_ETH_ACCOUNTING = (1 << 1),
    CB_TOK_ACCOUNTING = (1 << 2),
    CB_TRACES = (1 << 3),
} cachebits_t;
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
    wei_t extraValue1;
    wei_t extraValue2;
    gas_t gas;
    gas_t gasPrice;
    gas_t maxFeePerGas;
    gas_t maxPriorityFeePerGas;
    string_q input;
    uint8_t isError;
    uint8_t hasToken;
    uint8_t cachebits;
    uint8_t reserved2;
    CReceipt receipt;
    CTraceArray traces;
    CFunction articulatedTx;
    string_q compressedTx;
    CReconciliationArray statements;
    bool finalized;

  public:
    CTransaction(void);
    CTransaction(const CTransaction& tr);
    virtual ~CTransaction(void);
    CTransaction& operator=(const CTransaction& tr);

    DECLARE_NODE(CTransaction);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    const CBlock* pBlock;
    bool forEveryLog(LOGVISITFUNC func, void* data) const;
    bool forEveryTrace(TRACEVISITFUNC func, void* data) const;
    bool forEveryAppearanceInTx(APPEARANCEFUNC func, TRANSFUNC filt = NULL, void* data = NULL);
    bool forEveryUniqueAppearanceInTx(APPEARANCEFUNC func, TRANSFUNC filt = NULL, void* data = NULL);
    bool forEveryUniqueAppearanceInTxPerTx(APPEARANCEFUNC func, TRANSFUNC filt = NULL, void* data = NULL);
    bool loadTransAsPrefund(blknum_t bn, blknum_t txid, const address_t& addr, const wei_t& amount);
    bool loadTransAsBlockReward(blknum_t bn, blknum_t txid, const address_t& addr);
    bool loadTransAsUncleReward(blknum_t bn, blknum_t uncleBn, const address_t& addr);
    bool isReconciled(const address_t& accountedFor) const;
    bool readReconsFromCache(const address_t& accountedFor);
    void cacheIfReconciled(const address_t& accountedFor) const;
    string_q getReconcilationPath(const address_t& address) const;

    // EXISTING_CODE
    bool operator==(const CTransaction& it) const;
    bool operator!=(const CTransaction& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTransaction& v1, const CTransaction& v2);
    friend ostream& operator<<(ostream& os, const CTransaction& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTransaction& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    friend class CCachedAccount;
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
    extraValue1 = 0;
    extraValue2 = 0;
    gas = 0;
    gasPrice = 0;
    maxFeePerGas = 0;
    maxPriorityFeePerGas = 0;
    input = "";
    isError = 0;
    hasToken = 0;
    cachebits = CB_NONE;
    reserved2 = 0;
    receipt = CReceipt();
    traces.clear();
    articulatedTx = CFunction();
    compressedTx = "";
    statements.clear();
    finalized = false;

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
    extraValue1 = tr.extraValue1;
    extraValue2 = tr.extraValue2;
    gas = tr.gas;
    gasPrice = tr.gasPrice;
    maxFeePerGas = tr.maxFeePerGas;
    maxPriorityFeePerGas = tr.maxPriorityFeePerGas;
    input = tr.input;
    isError = tr.isError;
    hasToken = tr.hasToken;
    cachebits = tr.cachebits;
    reserved2 = tr.reserved2;
    receipt = tr.receipt;
    traces = tr.traces;
    articulatedTx = tr.articulatedTx;
    compressedTx = tr.compressedTx;
    statements = tr.statements;
    finalized = tr.finalized;

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
inline bool CTransaction::operator==(const CTransaction& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return (hash == it.hash);
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
// EXISTING_CODE
}  // namespace qblocks
