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
    SFAddress address;
    SFString name;
    SFString color;
    blknum_t firstBlock;
    blknum_t lastBlock;
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
    CAccountWatch(const SFString& _addr, const SFString& _name, blknum_t fB, blknum_t lB, const SFString& _color)
    : address(toLower(_addr)), name(_name), color(_color), firstBlock(fB), lastBlock(lB) {}
    SFString displayName(bool expand, bool terse, uint32_t w1=20, uint32_t w2=8) const { return displayName(expand, true,terse,w1,w2); }
    SFString displayName(bool expand, bool useColor, bool terse, uint32_t w1=20, uint32_t w2=8) const;
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

    address = "";
    name = "";
    color = "";
    firstBlock = 0;
    lastBlock = 0;
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

    address = ac.address;
    name = ac.name;
    color = ac.color;
    firstBlock = ac.firstBlock;
    lastBlock = ac.lastBlock;
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
extern SFUintBN getNodeBal(CBalanceHistoryArray& history, const SFAddress& addr, blknum_t blockNum);
extern void loadWatchList(const CToml& toml, CAccountWatchArray& watches, const SFString& key);
// EXISTING_CODE
}  // namespace qblocks

