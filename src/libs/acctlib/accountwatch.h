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
#include "abilib.h"
#include "transaction.h"
#include "incomestatement.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CAccountWatch;
typedef SFArrayBase<CAccountWatch>         CAccountWatchArray;
typedef SFList<CAccountWatch*>             CAccountWatchList;
typedef SFUniqueList<CAccountWatch*>       CAccountWatchListU;

// EXISTING_CODE
class CBalanceHistory {
public:
    blknum_t bn;
    SFUintBN balance;
    CBalanceHistory(void) { bn = 0; balance = 0; }
};
typedef SFArrayBase<CBalanceHistory> CBalanceHistoryArray;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccountWatch : public CBaseNode {
public:
    uint32_t id;
    SFAddress address;
    SFString name;
    SFString color;
    blknum_t firstBlock;
    blknum_t lastBlock;
    SFString status;
    bool deepScan;
    CIncomeStatement qbis;
    SFWei nodeBal;

public:
    CAccountWatch(void);
    CAccountWatch(const CAccountWatch& ac);
    virtual ~CAccountWatch(void);
    CAccountWatch& operator=(const CAccountWatch& ac);

    DECLARE_NODE(CAccountWatch);

    const CBaseNode *getObjectAt(const SFString& fieldName, uint32_t index) const override;

    // EXISTING_CODE
    CAccountWatch(uint32_t _id, const SFString& _addr, const SFString& _name, blknum_t fB, blknum_t lB, const SFString& _color)
    : id(_id), address(toLower(_addr)), name(_name), color(_color), firstBlock(fB), lastBlock(lB), status("") { }
    SFString displayName(bool terse, uint32_t w1=20, uint32_t w2=8) const { return displayName(true,terse,w1,w2); }
    SFString displayName(bool useColor, bool terse, uint32_t w1=20, uint32_t w2=8) const;
    SFBloom bloom;
    bool inBlock;
    CBalanceHistoryArray balanceHistory;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CAccountWatch& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CAccountWatch& ac);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(const CAccountWatch& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccountWatch::~CAccountWatch(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::Init(void) {
    CBaseNode::Init();

    id = 0;
    address = "";
    name = "";
    color = "";
    firstBlock = 0;
    lastBlock = 0;
    status = "";
    deepScan = 0;
    qbis.Init();
    nodeBal = 0;

    // EXISTING_CODE
    lastBlock = UINT_MAX;
    bloom = 0;
    inBlock = false;
    balanceHistory.Clear();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::Copy(const CAccountWatch& ac) {
    Clear();
    CBaseNode::Copy(ac);

    id = ac.id;
    address = ac.address;
    name = ac.name;
    color = ac.color;
    firstBlock = ac.firstBlock;
    lastBlock = ac.lastBlock;
    status = ac.status;
    deepScan = ac.deepScan;
    qbis = ac.qbis;
    nodeBal = ac.nodeBal;

    // EXISTING_CODE
    lastBlock = ac.lastBlock;
    bloom = ac.bloom;
    inBlock = ac.inBlock;
    balanceHistory = ac.balanceHistory;
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAccountWatch& CAccountWatch::operator=(const CAccountWatch& ac) {
    Copy(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAccountWatchArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CAccountWatchArray);
IMPLEMENT_ARCHIVE_LIST(CAccountWatchList);

//---------------------------------------------------------------------------
// EXISTING_CODE
SFUintBN getNodeBal(CBalanceHistoryArray& history, const SFAddress& addr, blknum_t blockNum);
// EXISTING_CODE
}  // namespace qblocks

