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
class CSlurpCache : public CCache {
  public:
    CAddressArray addrs;
    CMonitorCacheItemArray items;

  public:
    CSlurpCache(void);
    CSlurpCache(const CSlurpCache& sl);
    virtual ~CSlurpCache(void);
    CSlurpCache& operator=(const CSlurpCache& sl);

    DECLARE_NODE(CSlurpCache);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;
    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSlurpCache& it) const;
    bool operator!=(const CSlurpCache& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CSlurpCache& v1, const CSlurpCache& v2);
    friend ostream& operator<<(ostream& os, const CSlurpCache& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSlurpCache& sl);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSlurpCache::CSlurpCache(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSlurpCache::CSlurpCache(const CSlurpCache& sl) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(sl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSlurpCache::~CSlurpCache(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSlurpCache::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSlurpCache::initialize(void) {
    CCache::initialize();

    addrs.clear();
    items.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSlurpCache::duplicate(const CSlurpCache& sl) {
    clear();
    CCache::duplicate(sl);

    addrs = sl.addrs;
    items = sl.items;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSlurpCache& CSlurpCache::operator=(const CSlurpCache& sl) {
    duplicate(sl);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSlurpCache::operator==(const CSlurpCache& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSlurpCache& v1, const CSlurpCache& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSlurpCache> CSlurpCacheArray;
extern CArchive& operator>>(CArchive& archive, CSlurpCacheArray& array);
extern CArchive& operator<<(CArchive& archive, const CSlurpCacheArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_SLURPCACHE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
