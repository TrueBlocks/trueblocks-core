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
#include "entitycacheitem.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CEntityCache : public CCache {
  public:
    CEntityCacheItemArray items;

  public:
    CEntityCache(void);
    CEntityCache(const CEntityCache& en);
    virtual ~CEntityCache(void);
    CEntityCache& operator=(const CEntityCache& en);

    DECLARE_NODE(CEntityCache);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CEntityCache& it) const;
    bool operator!=(const CEntityCache& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CEntityCache& v1, const CEntityCache& v2);
    friend ostream& operator<<(ostream& os, const CEntityCache& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEntityCache& en);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEntityCache::CEntityCache(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEntityCache::CEntityCache(const CEntityCache& en) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(en);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEntityCache::~CEntityCache(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEntityCache::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEntityCache::initialize(void) {
    CCache::initialize();

    items.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEntityCache::duplicate(const CEntityCache& en) {
    clear();
    CCache::duplicate(en);

    items = en.items;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEntityCache& CEntityCache::operator=(const CEntityCache& en) {
    duplicate(en);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEntityCache::operator==(const CEntityCache& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEntityCache& v1, const CEntityCache& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CEntityCache> CEntityCacheArray;
extern CArchive& operator>>(CArchive& archive, CEntityCacheArray& array);
extern CArchive& operator<<(CArchive& archive, const CEntityCacheArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ENTITYCACHE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
