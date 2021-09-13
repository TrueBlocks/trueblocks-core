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
#include "cache.h"
#include "collectioncacheitem.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CCollectionCache : public CCache {
  public:
    CCollectionCacheItemArray items;

  public:
    CCollectionCache(void);
    CCollectionCache(const CCollectionCache& co);
    virtual ~CCollectionCache(void);
    CCollectionCache& operator=(const CCollectionCache& co);

    DECLARE_NODE(CCollectionCache);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCollectionCache& it) const;
    bool operator!=(const CCollectionCache& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CCollectionCache& v1, const CCollectionCache& v2);
    friend ostream& operator<<(ostream& os, const CCollectionCache& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCollectionCache& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCollectionCache::CCollectionCache(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCollectionCache::CCollectionCache(const CCollectionCache& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCollectionCache::~CCollectionCache(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCollectionCache::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCollectionCache::initialize(void) {
    CCache::initialize();

    items.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCollectionCache::duplicate(const CCollectionCache& co) {
    clear();
    CCache::duplicate(co);

    items = co.items;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCollectionCache& CCollectionCache::operator=(const CCollectionCache& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCollectionCache::operator==(const CCollectionCache& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCollectionCache& v1, const CCollectionCache& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCollectionCache> CCollectionCacheArray;
extern CArchive& operator>>(CArchive& archive, CCollectionCacheArray& array);
extern CArchive& operator<<(CArchive& archive, const CCollectionCacheArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CCollectionCache& col);
extern CArchive& operator>>(CArchive& archive, CCollectionCache& col);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_COLLECTIONCACHE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
