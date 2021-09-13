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
#include "etherlib.h"
#include "cachebase.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CCollectionCacheItem : public CCacheBase {
  public:
    string_q type;

  public:
    CCollectionCacheItem(void);
    CCollectionCacheItem(const CCollectionCacheItem& co);
    virtual ~CCollectionCacheItem(void);
    CCollectionCacheItem& operator=(const CCollectionCacheItem& co);

    DECLARE_NODE(CCollectionCacheItem);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCollectionCacheItem& it) const;
    bool operator!=(const CCollectionCacheItem& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CCollectionCacheItem& v1, const CCollectionCacheItem& v2);
    friend ostream& operator<<(ostream& os, const CCollectionCacheItem& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCollectionCacheItem& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCollectionCacheItem::CCollectionCacheItem(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCollectionCacheItem::CCollectionCacheItem(const CCollectionCacheItem& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCollectionCacheItem::~CCollectionCacheItem(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCollectionCacheItem::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCollectionCacheItem::initialize(void) {
    CCacheBase::initialize();

    type = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCollectionCacheItem::duplicate(const CCollectionCacheItem& co) {
    clear();
    CCacheBase::duplicate(co);

    type = co.type;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCollectionCacheItem& CCollectionCacheItem::operator=(const CCollectionCacheItem& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCollectionCacheItem::operator==(const CCollectionCacheItem& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCollectionCacheItem& v1, const CCollectionCacheItem& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCollectionCacheItem> CCollectionCacheItemArray;
extern CArchive& operator>>(CArchive& archive, CCollectionCacheItemArray& array);
extern CArchive& operator<<(CArchive& archive, const CCollectionCacheItemArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CCollectionCacheItem& col);
extern CArchive& operator>>(CArchive& archive, CCollectionCacheItem& col);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_COLLECTIONCACHEITEM;

//---------------------------------------------------------------------------
extern string_q nextCachebaseChunk(const string_q& fieldIn, const void* data);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
