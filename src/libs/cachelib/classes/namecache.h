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
#include "monitorcacheitem.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CNameCache : public CCache {
  public:
    CAddressArray addrs;
    CNameCacheItemArray items;

  public:
    CNameCache(void);
    CNameCache(const CNameCache& na);
    virtual ~CNameCache(void);
    CNameCache& operator=(const CNameCache& na);

    DECLARE_NODE(CNameCache);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;
    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CNameCache& it) const;
    bool operator!=(const CNameCache& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CNameCache& v1, const CNameCache& v2);
    friend ostream& operator<<(ostream& os, const CNameCache& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNameCache& na);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNameCache::CNameCache(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNameCache::CNameCache(const CNameCache& na) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(na);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNameCache::~CNameCache(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNameCache::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNameCache::initialize(void) {
    CCache::initialize();

    addrs.clear();
    items.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNameCache::duplicate(const CNameCache& na) {
    clear();
    CCache::duplicate(na);

    addrs = na.addrs;
    items = na.items;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNameCache& CNameCache::operator=(const CNameCache& na) {
    duplicate(na);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CNameCache::operator==(const CNameCache& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNameCache& v1, const CNameCache& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNameCache> CNameCacheArray;
extern CArchive& operator>>(CArchive& archive, CNameCacheArray& array);
extern CArchive& operator<<(CArchive& archive, const CNameCacheArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CNameCache& nam);
extern CArchive& operator>>(CArchive& archive, CNameCache& nam);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_NAMECACHE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
