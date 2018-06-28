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
#include <vector>
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAcctCacheItem : public CBaseNode {
public:
    uint64_t blockNum;
    uint64_t transIndex;

public:
    CAcctCacheItem(void);
    CAcctCacheItem(const CAcctCacheItem& ac);
    virtual ~CAcctCacheItem(void);
    CAcctCacheItem& operator=(const CAcctCacheItem& ac);

    DECLARE_NODE(CAcctCacheItem);

    // EXISTING_CODE
    CAcctCacheItem(uint64_t b, uint64_t t) : blockNum(b), transIndex(t) {}
    explicit CAcctCacheItem(string_q& str);
    // EXISTING_CODE
    bool operator==(const CAcctCacheItem& item);
    bool operator!=(const CAcctCacheItem& item) { return !operator==(item); }
    friend bool operator<(const CAcctCacheItem& v1, const CAcctCacheItem& v2);
    friend ostream& operator<<(ostream& os, const CAcctCacheItem& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAcctCacheItem& ac);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAcctCacheItem::CAcctCacheItem(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAcctCacheItem::CAcctCacheItem(const CAcctCacheItem& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAcctCacheItem::~CAcctCacheItem(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctCacheItem::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctCacheItem::initialize(void) {
    CBaseNode::initialize();

    blockNum = 0;
    transIndex = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctCacheItem::duplicate(const CAcctCacheItem& ac) {
    clear();
    CBaseNode::duplicate(ac);

    blockNum = ac.blockNum;
    transIndex = ac.transIndex;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAcctCacheItem& CAcctCacheItem::operator=(const CAcctCacheItem& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAcctCacheItem::operator==(const CAcctCacheItem& item) {
    return (
            blockNum == item.blockNum &&
            transIndex == item.transIndex
        );
}

//-------------------------------------------------------------------------
inline bool operator<(const CAcctCacheItem& v1, const CAcctCacheItem& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    if (v1.blockNum != v2.blockNum)
        return v1.blockNum < v2.blockNum;
    return v1.transIndex < v2.transIndex;
}

//---------------------------------------------------------------------------
typedef vector<CAcctCacheItem> CAcctCacheItemArray;
extern SFArchive& operator>>(SFArchive& archive, CAcctCacheItemArray& array);
extern SFArchive& operator<<(SFArchive& archive, const CAcctCacheItemArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

