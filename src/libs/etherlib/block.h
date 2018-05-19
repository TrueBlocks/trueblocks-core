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

namespace qblocks {

//--------------------------------------------------------------------------
class CBlock;
typedef SFArrayBase<CBlock>         CBlockArray;
typedef SFList<CBlock*>             CBlockList;
typedef SFUniqueList<CBlock*>       CBlockListU;

// EXISTING_CODE
typedef bool (*ADDRESSFUNC)(blknum_t bn, blknum_t tx, blknum_t tc, const SFAddress& addr, void *data);
typedef bool (*TRANSFUNC)(const CTransaction *trans, void *data);
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBlock : public CBaseNode {
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
    CBlock(void);
    CBlock(const CBlock& bl);
    virtual ~CBlock(void);
    CBlock& operator=(const CBlock& bl);

    DECLARE_NODE(CBlock);

    const CBaseNode *getObjectAt(const SFString& fieldName, uint32_t index) const override;

    // EXISTING_CODE
    bool forEveryAddress      (ADDRESSFUNC func, TRANSFUNC filt, void *data);
    bool forEveryUniqueAddress(ADDRESSFUNC func, TRANSFUNC filt, void *data);
    bool operator==(const CBlock& bl) const;
    bool operator!=(const CBlock& bl) const { return !operator==(bl); }
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CBlock& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CBlock& bl);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBlock::CBlock(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBlock::CBlock(const CBlock& bl) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(bl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBlock::~CBlock(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlock::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlock::Init(void) {
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
inline void CBlock::Copy(const CBlock& bl) {
    Clear();
    CBaseNode::Copy(bl);

    gasLimit = bl.gasLimit;
    gasUsed = bl.gasUsed;
    hash = bl.hash;
    blockNumber = bl.blockNumber;
    parentHash = bl.parentHash;
    miner = bl.miner;
    difficulty = bl.difficulty;
    price = bl.price;
    finalized = bl.finalized;
    timestamp = bl.timestamp;
    transactions = bl.transactions;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBlock& CBlock::operator=(const CBlock& bl) {
    Copy(bl);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CBlockArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CBlockArray);
IMPLEMENT_ARCHIVE_LIST(CBlockList);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CBlock& blo);
extern SFArchive& operator>>(SFArchive& archive, CBlock& blo);

//---------------------------------------------------------------------------
// EXISTING_CODE
inline blknum_t bnFromPath(const SFString& path) {
    SFString p = path.Substitute(".bin","");
    p.Reverse(); p = nextTokenClear(p, '/'); p.Reverse();
    return toUnsigned(p);
}

//---------------------------------------------------------------------------
inline bool isBlockFinal(timestamp_t ts_block, timestamp_t ts_chain, timestamp_t seconds = (60 * 4)) { // default to ten minutes
    // If the distance from the front of the node's current view of the front of the chain
    // is more than the numbers of seconds provided, consider the block final (even if it isn't
    // in a perfectly mathematical sense
    return ((ts_chain - ts_block) > seconds);
}
extern bool isPotentialAddr(SFUintBN test, SFAddress& addrOut);
extern void processPotentialAddrs(blknum_t bn, blknum_t tx, blknum_t tc, const SFString& potList, ADDRESSFUNC func, void *data);
// EXISTING_CODE
}  // namespace qblocks

