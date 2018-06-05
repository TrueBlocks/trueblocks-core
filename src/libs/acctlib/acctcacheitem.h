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
#include "etherlib.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CAcctCacheItem;
typedef SFArrayBase<CAcctCacheItem>         CAcctCacheItemArray;
typedef SFList<CAcctCacheItem*>             CAcctCacheItemList;
typedef SFUniqueList<CAcctCacheItem*>       CAcctCacheItemListU;

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
    CAcctCacheItem(SFString& str);
    bool operator==(const CAcctCacheItem& item) {
        return (blockNum == item.blockNum && transIndex == item.transIndex);
    }
    bool operator!=(const CAcctCacheItem& item) { return !operator==(item); }
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CAcctCacheItem& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CAcctCacheItem& ac);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAcctCacheItem::CAcctCacheItem(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAcctCacheItem::CAcctCacheItem(const CAcctCacheItem& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAcctCacheItem::~CAcctCacheItem(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctCacheItem::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctCacheItem::Init(void) {
    CBaseNode::Init();

    blockNum = 0;
    transIndex = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctCacheItem::Copy(const CAcctCacheItem& ac) {
    Clear();
    CBaseNode::Copy(ac);

    blockNum = ac.blockNum;
    transIndex = ac.transIndex;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAcctCacheItem& CAcctCacheItem::operator=(const CAcctCacheItem& ac) {
    Copy(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAcctCacheItemArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CAcctCacheItemArray);
IMPLEMENT_ARCHIVE_LIST(CAcctCacheItemList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

