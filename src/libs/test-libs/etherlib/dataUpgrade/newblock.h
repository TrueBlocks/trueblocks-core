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
#include "transaction.h"
#include "block.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CNewBlock;
typedef SFArrayBase<CNewBlock>         CNewBlockArray;
typedef SFList<CNewBlock*>             CNewBlockList;
typedef SFUniqueList<CNewBlock*>       CNewBlockListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CNewBlock : public CBaseNode {
public:
    SFUint32 gasLimit;
    SFUint32 gasUsed;
    SFHash hash;
    SFBloom logsBloom;
    SFUint32 blockNumber;
    SFHash parentHash;
    SFUint32 timestamp;
    CTransactionArray transactions;
    SFAddress miner;
    SFUint32 size;

public:
    CNewBlock(void);
    CNewBlock(const CNewBlock& ne);
    ~CNewBlock(void);
    CNewBlock& operator=(const CNewBlock& ne);

    DECLARE_NODE(CNewBlock);

    // EXISTING_CODE
    CNewBlock(const CBlock& block);
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CNewBlock& ne);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewBlock::CNewBlock(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewBlock::CNewBlock(const CNewBlock& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewBlock::~CNewBlock(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBlock::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBlock::Init(void) {
    CBaseNode::Init();

    gasLimit = 0;
    gasUsed = 0;
    hash = "";
    logsBloom = 0;
    blockNumber = 0;
    parentHash = "";
    timestamp = 0;
    transactions.Clear();
    miner = "";
    size = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewBlock::Copy(const CNewBlock& ne) {
    Clear();
    CBaseNode::Copy(ne);

    gasLimit = ne.gasLimit;
    gasUsed = ne.gasUsed;
    hash = ne.hash;
    logsBloom = ne.logsBloom;
    blockNumber = ne.blockNumber;
    parentHash = ne.parentHash;
    timestamp = ne.timestamp;
    transactions = ne.transactions;

    // EXISTING_CODE
#if 0
    author = ne.author;
    difficulty = ne.difficulty;
    extraData = ne.extraData;
    miner = ne.miner;
    mixHash = ne.mixHash;
    nonce = ne.nonce;
    receiptRoot = ne.receiptRoot;
    receiptsRoot = ne.receiptsRoot;
    sealFields = ne.sealFields;
    sha3Uncles = ne.sha3Uncles;
    size = ne.size;
    stateRoot = ne.stateRoot;
    totalDifficulty = ne.totalDifficulty;
    transactionsRoot = ne.transactionsRoot;
    uncles = ne.uncles;
#endif
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewBlock& CNewBlock::operator=(const CNewBlock& ne) {
    Copy(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString CNewBlock::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CNewBlockArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CNewBlockArray);
IMPLEMENT_ARCHIVE_LIST(CNewBlockList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CNewBlock& newp);
extern SFArchive& operator>>(SFArchive& archive, CNewBlock& newp);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

