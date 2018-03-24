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
#include "etherlib.h"
#include "transaction.h"

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
    SFGas gasLimit;
    SFGas gasUsed;
    SFHash hash;
    blknum_t blockNumber;
    SFHash parentHash;
    SFAddress miner;
    uint64_t difficulty;
    double price;
    bool finalized;
    timestamp_t timestamp;
    CTransactionArray transactions;

public:
    CNewBlock(void);
    CNewBlock(const CNewBlock& ne);
    virtual ~CNewBlock(void);
    CNewBlock& operator=(const CNewBlock& ne);

    DECLARE_NODE(CNewBlock);

    const CBaseNode *getObjectAt(const SFString& fieldName, uint32_t index) const override;

    // EXISTING_CODE
    CNewBlock(const CBlock& block);
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CNewBlock& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CNewBlock& ne);
    bool readBackLevel(SFArchive& archive) override;

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
    blockNumber = 0;
    parentHash = "";
    miner = "";
    difficulty = 0;
    price = 0.0;
    finalized = 0;
    timestamp = 0;
    transactions.Clear();

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
    blockNumber = ne.blockNumber;
    parentHash = ne.parentHash;
    miner = ne.miner;
    difficulty = ne.difficulty;
    price = ne.price;
    finalized = ne.finalized;
    timestamp = ne.timestamp;
    transactions = ne.transactions;

    // EXISTING_CODE
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
IMPLEMENT_ARCHIVE_ARRAY(CNewBlockArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CNewBlockArray);
IMPLEMENT_ARCHIVE_LIST(CNewBlockList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CNewBlock& newp);
extern SFArchive& operator>>(SFArchive& archive, CNewBlock& newp);

//---------------------------------------------------------------------------
// EXISTING_CODE
bool readOneNewBlock_fromBinary(CNewBlock& block, const SFString& fileName);
bool readOneNewBlock_fromJson(CNewBlock& block, const SFString& fileName);
// EXISTING_CODE

