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
#include <vector>
#include <map>
#include "abilib.h"
#include "transaction.h"
#include "incomestatement.h"
#include "balancehistory.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccountWatch : public CBaseNode {
public:
    address_t address;
    string_q name;
    string_q color;
    blknum_t firstBlock;
    blknum_t lastBlock;
    bool deepScan;
    CIncomeStatement qbis;
    CBalanceHistoryArray balanceHistory;
    wei_t nodeBal;

public:
    CAccountWatch(void);
    CAccountWatch(const CAccountWatch& ac);
    virtual ~CAccountWatch(void);
    CAccountWatch& operator=(const CAccountWatch& ac);

    DECLARE_NODE(CAccountWatch);

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    CAccountWatch(const string_q& _addr, const string_q& _name, blknum_t fB, blknum_t lB, const string_q& _color)
    : address(toLower(_addr)), name(_name), color(_color), firstBlock(fB), lastBlock(lB) {}
    string_q displayName(bool expand, bool terse, size_t w1 = 20, size_t w2 = 8) const
        { return displayName(expand, true, terse, w1, w2); }
    string_q displayName(bool expand, bool useColor, bool terse, size_t w1 = 20, size_t w2 = 8) const;
    bloom_t bloom;
    bool inBlock;
    // EXISTING_CODE
    bool operator==(const CAccountWatch& item) const;
    bool operator!=(const CAccountWatch& item) const { return !operator==(item); }
    friend bool operator<(const CAccountWatch& v1, const CAccountWatch& v2);
    friend ostream& operator<<(ostream& os, const CAccountWatch& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAccountWatch& ac);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(const CAccountWatch& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccountWatch::~CAccountWatch(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::initialize(void) {
    CBaseNode::initialize();

    address = "";
    name = "";
    color = "";
    firstBlock = 0;
    lastBlock = 0;
    deepScan = 0;
    qbis.initialize();
    balanceHistory.clear();
    nodeBal = 0;

    // EXISTING_CODE
    lastBlock = UINT_MAX;
    bloom = 0;
    inBlock = false;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::duplicate(const CAccountWatch& ac) {
    clear();
    CBaseNode::duplicate(ac);

    address = ac.address;
    name = ac.name;
    color = ac.color;
    firstBlock = ac.firstBlock;
    lastBlock = ac.lastBlock;
    deepScan = ac.deepScan;
    qbis = ac.qbis;
    balanceHistory = ac.balanceHistory;
    nodeBal = ac.nodeBal;

    // EXISTING_CODE
    lastBlock = ac.lastBlock;
    bloom = ac.bloom;
    inBlock = ac.inBlock;
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAccountWatch& CAccountWatch::operator=(const CAccountWatch& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAccountWatch::operator==(const CAccountWatch& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAccountWatch& v1, const CAccountWatch& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAccountWatch> CAccountWatchArray;
extern CArchive& operator>>(CArchive& archive, CAccountWatchArray& array);
extern CArchive& operator<<(CArchive& archive, const CAccountWatchArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern biguint_t getNodeBal(CBalanceHistoryArray& history, const address_t& addr, blknum_t blockNum);
extern void loadWatchList(const CToml& toml, CAccountWatchArray& watches, const string_q& key);
// EXISTING_CODE
}  // namespace qblocks

